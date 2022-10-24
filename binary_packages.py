def unique(json1, json2):
    d1 = dict()
    keys = json1[0].keys()
    for i in range(len(json1)):
        name = json1[i]["name"]

        if name in d1.keys():
            d1[name].append(json1[i])
        else:
            d1[name] = [json1[i]]

    i = 0
    l2 = len(json2)
    while i < l2:  # l2
        name = json2[i]["name"]
        if name in d1.keys():
            for j in range(0, len(d1[name])):
                compare = 0
                for key in keys:
                    if d1[name][j][key] == json2[i][key]:
                        compare += 1
                    else:
                        break
                if compare == len(keys):
                    del d1[name][j]
                    del json2[i]
                    i -= 1
                    l2 = len(json2)
                    break
        i += 1
    new_json1 = []
    for key in d1.keys():
        for value in d1[key]:
            new_json1.append(value)
    return new_json1, json2


def cmp(val1, val2):
    val1 = val1.split('.')
    val2 = val2.split('.')
    for i in range(max(len(val1), len(val2))):
        if i == len(val1):
            break
        if i >= len(val2):
            return True
        if val1[i] > val2[i]:
            return True
        elif val1[i] < val2[i]:
            return False
    return False


def check_vers_release(json1, json2):
    l1 = len(json1)
    arch_vers_json2 = dict()
    for i in range(len(json2)):
        arch = json2[i]["arch"]
        name = json2[i]["name"]
        arch = arch + name
        if arch in arch_vers_json2.keys():
            if cmp(json2[i]['version'], arch_vers_json2[arch]['version']):
                arch_vers_json2[arch]['version'] = json2[i]['version']
            if cmp(json2[i]['release'], arch_vers_json2[arch]['release']):
                arch_vers_json2[arch]['release'] = json2[i]['release']
        else:
            arch_vers_json2[arch] = {'version': json2[i]['version'],
                                     'release': json2[i]['release']}
    i = 0
    while i < l1:  # l1
        arch = json1[i]["arch"]
        name = json1[i]["name"]
        arch = arch + name
        if arch not in arch_vers_json2.keys():
            del json1[i]
            i -= 1
            l1 = len(json1)
        else:
            if not cmp(json1[i]['version'], arch_vers_json2[arch]['version']):
                del json1[i]
                i -= 1
                l1 = len(json1)
            elif not cmp(json1[i]['release'], arch_vers_json2[arch]['release']):
                del json1[i]
                i -= 1
                l1 = len(json1)
        i += 1
    return json1


def get_json(branch_one='p10', branch_two='p9'):
    import requests
    import json
    # branch_one='p10'
    # branch_two='p9'
    url1 = None
    url2 = None
    try:
        url1 = "https://rdb.altlinux.org/api/export/branch_binary_packages/" + branch_one
    except:
        print("Not right name of branch one: "+branch_one)
        exit()
    try:
        url2 = "https://rdb.altlinux.org/api/export/branch_binary_packages/" + branch_two
    except:
        print("Not right name of branch two: " + branch_two)
        exit()
    r1 = requests.get(url1)
    json1 = r1.json()
    r2 = requests.get(url2)
    json2 = r2.json()
    find_json1, find_json2 = unique(json1["packages"].copy(), json2["packages"].copy())
    cmp_json_arch = check_vers_release(json1["packages"].copy(), json2["packages"].copy())
    my_json = {
        f'task1{branch_one}_{branch_two}_length': len(find_json1),
        f'task1{branch_one}_{branch_two}_packages': find_json1,
        f'task2{branch_one}_{branch_two}_length': len(find_json2),
        f'task2{branch_one}_{branch_two}_packages': find_json2,
        f'task3{branch_one}_{branch_two}_length': len(cmp_json_arch),
        f'task3{branch_one}_{branch_two}_packages': cmp_json_arch
    }

    with open(f"packages_{branch_one}_{branch_two}.json", "w") as fp:
        json.dump(my_json, fp)

    return my_json
