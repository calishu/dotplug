Concept JSON Layout

```json
// file structure (~/.config/dotplug)
"root.json"
"profiles" \
    "profile-1 hash" \
        "profile.json"
        "items" \
            "item-1 hash" \
                // user config
```

```json
// root json
{
    "active_config": "id of the active config",
    "profiles": [ "id of profile" ]
}
```

```json
// profile json
{
    "name": "Hello, Profile!",
    "author": "Hello, Author!",
    "id": "sha256 of the profile name",
    "items": [
        {
            "name": "Hello, Item!",
            "id": "sha256 of the profile name",
            "destination": "/home/user/.config/item/",
        }
    ]
}
```
