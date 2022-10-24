import argparse
from binary_packages import get_json

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Get binary packages from branches')
    parser.add_argument('branches', type=str, help='Names of the first branch and second branch (default: p10 and p9)', default=['p10','p9'], nargs='*')
    args = parser.parse_args()

    branch_one = 'p10'
    branch_two = 'p9'
    if len(args.branches) == 1:
        branch_one = args.branches[0]
    elif len(args.branches) > 2:
        print("Error: Too much args. You need to enter 1 or 2 args")
        exit()
    else:
        branch_one = args.branches[0]
        branch_two = args.branches[1]
    #print(args.branches)
    json_result = get_json(branch_one=branch_one, branch_two=branch_two)
    print(json_result)