import random

size_bytes = 23*1024*1024
size_rec_mean = 10 + 5 + 3
records = int(size_bytes / size_rec_mean)

sym_str = ' abcdefghigklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'

for i in range(records):
    weight = random.randint(1, 100000)
    text_len = random.randint(5, 15)
    s = ''
    for c in range(text_len):
        s += sym_str[random.randrange(len(sym_str))]
    print(f'{s}, {weight}')



