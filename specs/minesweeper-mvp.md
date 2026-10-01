# Spec: Console Minesweeper MVP

## Objective

Build a single-session, text-based C++ Minesweeper game for the assignment. A
player selects board positions by 1-based row and column coordinates. The game
ends when the player reveals a mine or has revealed all safe cells.

Success is a simple 5x5 game with five randomly placed mines, clear input
validation, and no mechanics beyond the stated MVP.

## Tech Stack

- C++ compiled with `g++`
- C++ standard library only, including `<array>` and `<random>` as needed
- No external libraries, build system, or test framework

## Commands

Build and run the existing interactive workflow:

```bash
./test_runner.sh
```

Build without launching the application:

```bash
g++ *.cpp -o app
```

Run the built application interactively:

```bash
./app
```

## Project Structure

```text
main.cpp                 Console application and all MVP game logic
test_runner.sh           Compiles root-level C++ sources and starts the game
specs/minesweeper-mvp.md MVP specification and implementation plan
tests/                   Reserved for test code; no automated harness exists yet
```

The implementation remains in `main.cpp` so it is compatible with the
repository's current single-source-file layout and `g++ *.cpp` command.

## Code Style

Use small, named helper functions and fixed constants. Keep board state in a
plain cell type; do not introduce classes, inheritance, dynamic allocation, or
configuration abstractions for this MVP.

```cpp
constexpr int kBoardSize = 5;
constexpr int kMineCount = 5;

struct Cell {
  bool isMine = false;
  bool isRevealed = false;
  int adjacentMines = 0;
};
```

- Use `k`-prefixed names for compile-time constants.
- Use descriptive camelCase names for variables and functions.
- Convert from 1-based user coordinates to zero-based array indices only after
  validating the entered values.
- Keep input, rendering, setup, and turn-resolution helpers focused on one job.

## Functional Requirements

1. Create a 5x5 board containing exactly five mines in distinct random cells.
2. Initially render every board cell as `#`.
3. Prompt the player for a row and column, both using 1-based coordinates.
4. Reject malformed input, coordinates outside 1 through 5, and already
   revealed cells. A rejected selection must not change board state.
5. Reveal a safe selected cell as its number of adjacent mines, including `0`.
6. Revealing a mine ends the game, renders all mines visibly (for example,
   `*`), and prints a loss message.
7. Win and print a win message after revealing all 20 non-mine cells.

## Intentional Scope Boundaries

- A selected safe zero cell reveals only that cell; there is no automatic
  neighboring-cell expansion.
- There are no flags, difficulty settings, menu, restart loop, saved games,
  GUI, or external dependencies.
- The application runs one game session and exits after a win or loss.

## Testing Strategy

The existing `test_runner.sh` is an interactive compile-and-run script, not an
automated test framework. Verify the MVP manually after a successful build:

- Start a new game and confirm every cell is initially `#`.
- Confirm safe cells reveal a number between `0` and `8`.
- Submit malformed input, an out-of-range coordinate, and a duplicate safe-cell
  selection; confirm each is rejected and the game continues.
- In separate runs, confirm a mine produces the final mine display and loss
  message, and that revealing 20 safe cells produces the win message.

Do not add a second test executable or framework as part of this MVP. Root-level
test sources with another `main` would conflict with `g++ *.cpp`.

## Boundaries

- Always: use fixed 5x5/5-mine constants, validate every selection, preserve
  the repository's existing compile/run workflow, and build before handoff.
- Ask first: changing `test_runner.sh`, adding a build system or dependency,
  splitting the program into multiple source files, or adding automated tests.
- Never: add flags, automatic zero-cell expansion, difficulty selection,
  menus/restarts, graphical UI, external libraries, or unrelated refactors.

## Implementation Plan and Tasks

### Task 1: Board setup

- [ ] Define the fixed constants, cell state, fixed-size board, unique mine
  placement, and adjacent-mine calculation in `main.cpp`.
  - Acceptance: every game has exactly five mines and every safe cell has a
    computed count from `0` through `8`.
  - Verify: build with `g++ *.cpp -o app`; inspect several game runs.
  - Files: `main.cpp`

### Task 2: Rendering and validated selection

- [ ] Render hidden and revealed cells and repeatedly accept one valid row and
  column selection.
  - Acceptance: hidden cells show `#`; valid selections are 1-based; malformed,
    out-of-range, and duplicate selections are rejected without state changes.
  - Verify: run `./test_runner.sh` and exercise valid and invalid input.
  - Files: `main.cpp`

### Task 3: Win and loss resolution

- [ ] Reveal safe cells, count safe reveals, and terminate on a mine or a full
  safe-board reveal.
  - Acceptance: a mine reveals all mines and loses; 20 safe reveals wins.
  - Verify: manually complete both game-ending paths over one or more runs.
  - Files: `main.cpp`

### Checkpoint: MVP complete

- [ ] `g++ *.cpp -o app` completes without errors.
- [ ] The manual scenarios in Testing Strategy pass.
- [ ] No out-of-scope mechanics or dependencies were introduced.

## Success Criteria

- The application is a console/text-based C++ program on a fixed 5x5 board.
- Exactly five mines are randomly and uniquely placed each game.
- All stated input, reveal, win, and loss behaviors work.
- The implementation remains standard-library-only and limited to the MVP.

## Open Questions

None. The following decisions are explicit for the MVP: render a revealed zero
as `0`, do not auto-expand zero cells, and use manual verification with the
current interactive script.
