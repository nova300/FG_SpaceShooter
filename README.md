Space War

A 2D Space shooter by Max Fagerström

**Initial performance considerations**

My project is implemented in C++ using the library SDL. Performance considerations when first building up the base game was:

- Minimizing heap allocations
- Using a SIMD math library
- Minimize running of heavy code every frame

The base game was built up with these considerations and in an mostly object oriented fashion.

**Measuring performance**

After the game was built to a playable state I used visual studio tools to measure performance and detect hotspots. This was done by disabling player collision and spawning about 1000 enemies.

Hotspots detected were collision detection and text drawing. Collision conducts a distance calculation for each enemy, and thus time complexity grows with the number of enemies. This might even be squared growth as each enemy searches the entire set of enemies. I consider this the main performance bottleneck in this application.

Text drawing on the other hand is a less complex issue, and should be easily solved by limiting drawing to frames where the number has actually changed.

**Optimizing hotspots**

To optimize the bottleneck, I first made the score counter store the current displayed value and only update the graphics if the displayed value is different from the current value.

For the collisions a more extensive rewrite was required. Collision objects are now stored in a contiguous place in memory in a single vector, objects get an index for this vector when they request a new collider object and do all interactions by calling a function to get a pointer to the collider at that index if it exists. This is a more data oriented approach than previous and also allows me to have a single point for updates on all the colliders working in a contiguous space in memory. I also made a further optimization in that the colliders now use the squared distance when doing its calculations, this removes the need for many square root calculations.

**Measuring results**

Once again spawning a bunch of enemies and checking the results now showed internal sdl functions and a reduced cpu usage since earlier.
