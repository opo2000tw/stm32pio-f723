import json
from pathlib import Path

cwd_path = Path.joinpath(Path.cwd(), 'lib').joinpath('library.json')
cwd = str(cwd_path)

with open(cwd_path, 'r') as read_file:
    dict_data = json.load(read_file)

    # list all keys
    print('dict_data.keys = ', dict_data.keys())
    print(dict_data['name'])


jsObj = json.dumps(dict_data)
pass
