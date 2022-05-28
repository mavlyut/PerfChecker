import json
import sys


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Wrong argument count: ", len(sys.argv) != 4)
        sys.exit(1)

    json_fix = None
    json_base = None
    with open(sys.argv[1]) as fix:
        json_fix = json.load(fix)

    with open(sys.argv[2]) as base:
        json_base = json.load(base)
    
    multiplier = float(sys.argv[3])

    print("solution time: ", json_fix["benchmarks"][0]["real_time"])
    print("base time", json_base["benchmarks"][0]["real_time"])
    print("expected improvement multiplier: ", multiplier)

    if (float(json_fix["benchmarks"][0]["real_time"]) <= json_base["benchmarks"][0]["real_time"] / multiplier):
        print("OK")
    else:
        print("FAIL")
        sys.exit(1)
