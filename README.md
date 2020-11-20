- SubModule
```
### Remove
# Remove the submodule entry from .git/config
git submodule deinit -f path/to/submodule
# Remove the submodule directory from the superproject's .git/modules directory
rm -rf .git/modules/path/to/submodule
# Remove the entry in .gitmodules and remove the submodule directory located at path/to/submodule
git rm -f path/to/submodule

### Add
git submodule add path/to/submodule path/to/submodule

### Update
git submodule update --init
```

- CloudSync
``` Vscode
https://gist.github.com/opo2000tw/7a9a7ee1964ea33aee4231a2485ddc6c
```

- WSL
```
https://docs.microsoft.com/zh-tw/windows/wsl/install-win10
```

- Others
```
editconfig: .editorconfig
astyle: .astylerec or vscode setting
conda: environment.yml
```
