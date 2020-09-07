import json
import sys

if len(sys.argv) != 2:
    print("Usage: {} filename".format(sys.argv[0]), file=sys.stderr)
    exit(1)

with open(sys.argv[1]) as f:
    fileJson = json.load(f)

with open(sys.argv[1], "w") as f:
    json.dump(fileJson, f, indent=4, separators=(',', ':'))

exit(0)

