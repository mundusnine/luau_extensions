# Luau extensions

Bring back PUC Rio lua functionnality into luau. Mostly used for BYOGE.

- [X] Equivalent require
    - [ ] Optionnal "Sandbox" require with PhysFS
- [X] IO System access
    - [ ] Optionnal "Sandbox" io access with PhysFS

Dependencies:

To integrate Luau extensions into your CMake application projects, at the minimum you'll need to depend on Luau.Compiler and Luau.VM projects from the Luau repos. Luau.Compiler dependency will eventually be optionnal through a define.
