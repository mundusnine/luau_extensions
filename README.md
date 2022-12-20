# Luau extensions

Bring back PUC Rio lua functionality into luau. Mostly used for BYOGE.

- [X] Equivalent require
    - [ ] Optional "Sandbox" require with PhysFS
- [X] IO System access
    - [ ] Optional "Sandbox" io access with PhysFS
- [ ] LSP support for libs
    - [ ] iolib.d.luau

Dependencies:

To integrate Luau extensions into your CMake application projects, at the minimum you'll need to depend on Luau.Compiler and Luau.VM projects from the Luau repos. Luau.Compiler dependency will eventually be optional through a define.
