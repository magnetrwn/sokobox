# Levelset Structure

**Layout and structure of the levelset level containing files.**

## Levelset contents

A levelset is made up of:
+ a levelset descriptor in INI format, outlining all relevant settings regarding each of the levels, their order and their rules
+ each level's tiles serialized into Zstandard compressed binary data in separate files, for the `LEVEL_DATA` key to point to in each level section

The levelset files, descriptor and levels, are placed into a folder to hold the levelset data together. If distributed as a `.zip`, it should be extracted and placed as a folder in the location levelsets are held.

A levelset descriptor is structured as follows:
+ `[Set.Overview]`, an overview section containing settings shared by all levels 
+ `[Set.Levels.#]`, a section for each level settings, where `#` matches the `LEVEL_ID` integer

## Example levelset

### `LevelSet123/levelset.ini`

```ini
[Set.Overview]
LEVELSET_FORMAT = 0
LEVELSET_FORMAT_REV = 0
LEVELSET_RULESET = sokoban
LEVELSET_NAME = Test Levelset
LEVELSET_AUTHOR = Test Author
LEVELSET_DESCRIPTION = A test levelset for development purposes.
LEVELSET_COUNT = 2

[Set.Levels.0]
LEVEL_ID = 0
LEVEL_NAME = Test Level 1
LEVEL_DESCRIPTION = This is the first test level.
LEVEL_HINT = Try pushing the crate onto the target.
LEVEL_VALID = true
LEVEL_COMPLETE_TYPE = gem
LEVEL_COMPLETE_TIMEOUT_S = 0
LEVEL_WIDTH = 10
LEVEL_HEIGHT = 10
LEVEL_PLAYER_X = 1
LEVEL_PLAYER_Y = 1
LEVEL_DATA = level1.bin

[Set.Levels.1]
LEVEL_ID = 1
LEVEL_NAME = Test Level 2
LEVEL_DESCRIPTION = This is the second test level.
LEVEL_HINT = Another test string.
LEVEL_VALID = false
LEVEL_COMPLETE_TYPE = box
LEVEL_COMPLETE_TIMEOUT_S = 90
LEVEL_WIDTH = 10
LEVEL_HEIGHT = 10
LEVEL_PLAYER_X = 1
LEVEL_PLAYER_Y = 1
LEVEL_DATA = level2.bin
```