
# a number in base 8 represented as digits in base 10
# 123(base8) : 
# 3*8^0 + 2*8^1 + 1*8^2 = 83 (base10)

def base8_to_base10(x):
    result = 0
    place = 0
    while x != 0:
        digit = x % 10
        x = x / 10
        result += digit * (8 ** place)
        place += 1
    return result

    
# algorithm for converting x from base10 to base X
# 83 base10 to base 8
# - 83/8 = 10 (reminder 3)
# - 10/8 = 1 (reminder 2)
# - 1/8 = 0 (reminder 1)
# result 123

def base10_to_base8(x):
   result = 0
   place = 0
   while x != 0:
      reminder = x % 8
      x = x / 8
      result += reminder * (10 ** place)
      place += 1
   return result

