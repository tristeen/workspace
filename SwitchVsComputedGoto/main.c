//
//  main.c
//  SwitchVsComputedGoto
//
//  Created by tristeen on 10/22/16.
//  Copyright © 2016 Tristeen. All rights reserved.
//

// http://eli.thegreenplace.net/2012/07/12/computed-goto-for-efficient-dispatch-tables#id5
// compare switch as computed goto.
//
/* Why is it faster?
Further down in the post you'll find two "bonus" sections that contain annotated disassembly of the two functions shown above, compiled at the -O3 optimization level with GCC. It's there for the real low-level buffs among my readers, and as a future reference for myself. Here I aim to explain why the computed goto code is faster at a bit of a higher level, so if you feel there are not enough details, go over the disassembly in the bonus sections.

The computed goto version is faster because of two reasons:

The switch does a bit more per iteration because of bounds checking.
The effects of hardware branch prediction.
Doing less per iteration
If you examine the disassembly of the switch version, you'll see that it does the following per opcode:

Execute the operation itself (i.e. val *= 2 for OP_MUL2)
pc++
Check the contents of code[pc]. If within bounds (<= 6), proceed. Otherwise return from the function.
Jump through the jump table based on offset computed from code[pc].
On the other hand, the computed goto version does this:

Execute the operation itself
pc++
Jump through the jump table based on offset computed from code[pc].
The difference between the two is obviously the "bounds check" step of the switch. Why is it required? You may think that this is because of the default clause, but that isn't true. Even without the default clause, the compiler is forced to generate the bounds check for the switch statement to conform to the C standard. Quoting from C99:

If no converted case constant expression matches and there is no default label, no part of the switch body is executed.
Therefore, the standard forces the compiler to generate "safe" code for the switch. Safety, as usual, has cost, so the switch version ends up doing a bit more per loop iteration.

Branch prediction
Modern CPUs have deep instruction pipelines and go to great lengths ensuring that the pipelines stay as full as possible. One thing that can ruin a pipeline's day is a branch, which is why branch predictors exist. Put simply (read the linked Wikipedia article for more details), it's an algorithm used by the CPU to try to predict in advance whether a branch will be taken or not. Since a CPU can easily pre-fetch instructions from the branch's target, successful prediction can make the pre-fetched instructions valid and there is no need to fully flush the pipeline.


The thing with branch predictors is that they map branches based on their addresses. Since the switch statement has a single "master jump" that dispatches all opcodes, predicting its destination is quite difficult. On the other hand, the computed goto statement is compiled into a separate jump per opcode, so given that instructions often come in pairs it's much easier for the branch predictor to "home in" on the various jumps correctly.

Think about it this way: for each jump, the branch predictor keeps a prediction of where it will jump next. If there's a jump per opcode, this is equivalent to predicting the second opcode in an opcode pair, which actually has some chance of success from time to time. On the other hand, if there's just a single jump, the prediction is shared between all opcodes and they keep stepping on each other's toes with each iteration.

I can't say for sure which one of the two factors weighs more in the speed difference between the switch and the computed goto, but if I had to guess I'd say it's the branch prediction.*/


/*
 tristeendeMacBook-Pro:SwitchVsComputedGoto tristeen$ time ./cgoto.out
 
 real	0m1.197s
 user	0m1.181s
 sys	0m0.005s
 tristeendeMacBook-Pro:SwitchVsComputedGoto tristeen$ time ./cgoto.out
 
 real	0m1.152s
 user	0m1.138s
 sys	0m0.004s
 tristeendeMacBook-Pro:SwitchVsComputedGoto tristeen$ time ./cgoto.out
 
 real	0m1.160s
 user	0m1.143s
 sys	0m0.006s
 tristeendeMacBook-Pro:SwitchVsComputedGoto tristeen$ time ./cgoto.out
 
 real	0m1.164s
 user	0m1.148s
 sys	0m0.004s
 tristeendeMacBook-Pro:SwitchVsComputedGoto tristeen$ time ./cgoto.out
 
 real	0m1.146s
 user	0m1.130s
 sys	0m0.005s
 
 tristeendeMacBook-Pro:SwitchVsComputedGoto tristeen$ time ./switch.out
 
 real	0m1.514s
 user	0m1.500s
 sys	0m0.003s
 tristeendeMacBook-Pro:SwitchVsComputedGoto tristeen$ time ./switch.out
 
 real	0m1.531s
 user	0m1.517s
 sys	0m0.003s
 tristeendeMacBook-Pro:SwitchVsComputedGoto tristeen$ time ./switch.out
 
 real	0m1.505s
 user	0m1.491s
 sys	0m0.003s
 tristeendeMacBook-Pro:SwitchVsComputedGoto tristeen$ time ./switch.out
 
 real	0m1.494s
 user	0m1.477s
 sys	0m0.007s
 tristeendeMacBook-Pro:SwitchVsComputedGoto tristeen$ time ./switch.out
 
 real	0m1.534s
 user	0m1.517s
 sys	0m0.005s

*/

#include <stdio.h>


#define OP_HALT     0x0
#define OP_INC      0x1
#define OP_DEC      0x2
#define OP_MUL2     0x3
#define OP_DIV2     0x4
#define OP_ADD7     0x5
#define OP_NEG      0x6

int interp_switch(unsigned char* code, int initval) {
    int pc = 0;
    int val = initval;
    
    while (1) {
        switch (code[pc++]) {
            case OP_HALT:
                return val;
            case OP_INC:
                val++;
                break;
            case OP_DEC:
                val--;
                break;
            case OP_MUL2:
                val *= 2;
                break;
            case OP_DIV2:
                val /= 2;
                break;
            case OP_ADD7:
                val += 7;
                break;
            case OP_NEG:
                val = -val;
                break;
            default:
                return val;
        }
    }
}

int interp_cgoto(unsigned char* code, int initval) {
    /* The indices of labels in the dispatch_table are the relevant opcodes
     */
    static void* dispatch_table[] = {
        &&do_halt, &&do_inc, &&do_dec, &&do_mul2,
        &&do_div2, &&do_add7, &&do_neg};
#define DISPATCH() goto *dispatch_table[code[pc++]]
    
    int pc = 0;
    int val = initval;
    
    DISPATCH();
    // while (1) {
    do_halt:
        return val;
    do_inc:
        val++;
        DISPATCH();
    do_dec:
        val--;
        DISPATCH();
    do_mul2:
        val *= 2;
        DISPATCH();
    do_div2:
        val /= 2;
        DISPATCH();
    do_add7:
        val += 7;
        DISPATCH();
    do_neg:
        val = -val;
        DISPATCH();
    //}
}

int main(int argc, const char * argv[]) {
    unsigned char code[5] = {OP_INC, OP_INC, OP_DEC, OP_MUL2, OP_HALT};
    for(int i=0; i <= 100000000; ++i) {
        interp_switch(code, 1);
    }
    
    /*for(int i=0; i <= 100000000; ++i) {
        interp_cgoto(code, 1);
    }*/
    /*int v1 = interp_switch(code, 1);
    int v2 = interp_cgoto(code, 1);
    printf("%d %d\n", v1, v2);*/
    return 0;
}
