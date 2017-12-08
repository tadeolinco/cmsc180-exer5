import sys

n = int(sys.argv[1]);
total = n - 2
prods = [total for x in range(total)]

offset = 1
for i in range(total - 1, 0, -1):
  for j in range(offset, total):
    prods[j] *= i
  offset += 1

resistance = 1
for i in range(3, n + 1):
  resistance += (1 / (i - 1)) * prods[i - 3]

resistance = 1 / resistance
print(resistance)