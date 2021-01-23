# Configuration

### Adding your own effects to the server

The server can run any effect would run in a standard loop(). You'll just need to create an effect function in the main code and edit some HTML, easy!

### HTML

You'll need to edit some of the strings in [`include/WebStrings.h`](include/WebStrings.h`). Start by adding an index for your new effect:

```cpp
#define RACER_INDEX 8
// Add your effect here
#define YOUR_EFFECT 9
```

Give your effect a name:

```cpp
String effectString[] = {...,
                        "Neon Racers",
                        "YOUR_EFFECT"};
```

Add to the selection box:


```cpp
String effectSelect(uint8_t effect) {
    ...
    ptrString = R"=====(
<form action="/effect" method="get">
  <select name="effect" id="effect" onchange='if(this.value != 0) { this.form.submit(); }'>
    <option value=0>Choose effect...</option>
    ...
    <option value=10>YOUR_EFFECT</option>
  </select>
</form>
    ...
```
