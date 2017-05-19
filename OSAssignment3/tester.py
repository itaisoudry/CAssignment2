import subprocess
import os
import random
import string
import sys

programName = "./dispatcher"
test_file = "test_file.txt"

random_string = "".join([random.choice(string.ascii_lowercase) for _ in range(int(sys.argv[1]))])

count_of_letter = {letter:random_string.count(letter) for letter in string.ascii_lowercase}

with open(test_file, "w") as f:
    f.write(random_string)

f.close()

for letter in string.ascii_lowercase:
    tmp = subprocess.Popen([programName, letter, test_file], stdout=subprocess.PIPE)
    result = int([t for t in tmp.stdout.read().split() if t.isdigit()][0])
    if result == count_of_letter[letter]:
        print("SUCCESS! found " + str(letter) + " " + str(result) + " times!")
    else:
        print("FAILURE! found " + str(letter) + " only " + str(result) + "out of " + str(count_of_letter[letter]) + " times!")

os.system("rm " + test_file)
