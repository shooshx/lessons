
a = 0
b = 100

while a != b - 1:
    mid = (a + b)/2
    print a, b, " is it bigger than ", mid, " ? (y/n)"
    if raw_input() == 'y':
        a = mid
    else:
        b = mid
print "it is ", b

