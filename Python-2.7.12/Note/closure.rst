

def f1():
    i = 1
    def f2():
        print i
    i += 1
    f2()
    i += 1
    f2()

f1() 
# >>> 2
# >>> 3

'''
1.  code object for f1.
        cellvars ('i',)
    code object for f2.
        freevars ('i',)

2.  frame object for f1.
        i is cell object, whose pointer is holded by locals[x].
    frame object for f2.
        i, a free object (locals[xxxx]), who is loaded from func closure of func object f2, is the same with i in f1.
'''
