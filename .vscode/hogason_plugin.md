### vscode 常用配置

```json
"astyle.executable": "E:\\HuyaUtils\\AStyle\\bin\\AStyle.exe",
"astyle.cmd_options": [
    "--ascii",
    "--style=allman",
    "--indent-namespaces",
    "--pad-oper",
    "--align-pointer=name",
    "--align-reference=name",
    "--add-brackets",
    "--break-blocks",
    "--pad-paren-in",
    "--unpad-paren",
    "--max-code-length=160",
    "--fill-empty-lines"
],
"[cpp]": {
    "editor.defaultFormatter": "chiehyu.vscode-astyle"
}

"go.autocompleteUnimportedPackages": true,
```