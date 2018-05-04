

def base10_to_base8(x):
   result = 0
   place = 0
   while x != 0:
      m = x % 8
      x = x / 8
      result += m * (10 ** place)
      place += 1
   return result

def base8_to_base10(x):
    result = 0
    place = 0
    while x != 0:
        m = x % 10
        x = x / 10
        result += m * (8 ** place)
        place += 1
    return result