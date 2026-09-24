# Game of Chess

Game of Chess is a 3D chess game built with Unreal Engine 5.2. It combines a C++ chess rules and game-management layer with Unreal Engine Actors, Blueprints, materials, and UI assets to provide a complete human-versus-computer chess experience.

The player controls the White pieces from a fixed top-down camera, while the Black opponent is controlled by an AI player. The opponent can be configured to choose legal moves randomly or to evaluate candidate moves with a depth-limited minimax search.

## Features

- Complete 8x8 chessboard rendered as Unreal Engine Actors.
- Separate piece implementations for pawns, rooks, knights, bishops, queens, and kings.
- Human player versus AI opponent gameplay.
- Two AI modes:
  - **Random opponent**: selects a legal move at random.
  - **Minimax opponent**: searches candidate positions to a fixed depth of three plies and evaluates material advantage and check situations.
- Legal-move validation, including protection against moves that leave the moving side in check.
- Check, checkmate, and stalemate detection.
- Pawn promotion handling.
- Visual feedback for selectable moves, the last move, and kings under check.
- Turn, score, move, and game-state messages exposed to the Blueprint UI.
- Game restart and score reset actions.
- Move-history playback with controls to navigate to the beginning, previous move, next move, or end of the recorded game.
- Undo support for the most recent move.
- Blueprint-driven board, piece, HUD, popup, input, and material assets.

## Technology Stack

| Area | Technology |
| --- | --- |
| Engine | Unreal Engine 5.2 |
| Gameplay code | C++ |
| Visual scripting and UI | Unreal Engine Blueprints |
| Input | Enhanced Input and mouse click actions |
| Rendering | Unreal Engine desktop renderer with DirectX 12 configuration |
| Build system | UnrealBuildTool |
| Editor tooling | Modeling Tools Editor Mode plugin |
| Target platform | Desktop, with Windows settings included in the project configuration |

## Architecture

The project is organized around a small set of gameplay responsibilities:

### Game flow

`AGameManager` derives from `AGameModeBase` and coordinates the complete match lifecycle. It starts and restarts games, spawns the board and players, switches turns, triggers AI moves, updates scores and messages, and checks end-game conditions.

### Board and presentation

`ABoard` creates the 8x8 grid, spawns the visual piece Actors, maps board coordinates to world positions, and manages tile highlighting. The board uses a fixed camera positioned above the play area.

### Chess state and simulation

`VirtualBoard` stores a lightweight 8x8 representation of piece types, teams, and the last move notation. `VirtualBoardManager` operates on these representations to generate moves, simulate positions, validate checks, track history, and support undo and playback without depending on the rendered Actors.

### Players and AI

`AGoC_Player` provides the common player abstraction. `AGoC_HumanPlayer` handles the human side, while `AGoC_AIPlayer` implements random move selection and a depth-limited minimax search. The AI evaluates positions using material values and a check bonus.

### Pieces and tiles

Each chess piece has its own C++ class derived from `APiece`, with corresponding White and Black Blueprint subclasses used to assign visual assets and materials. `ATile` represents a board square and owns the visual state required for occupancy and highlighting.

### UI and input

The HUD and initialization popup are implemented as Blueprint assets. Game-manager functions are exposed with `BlueprintCallable` where appropriate, allowing the UI to display the current turn, move, score, and general status messages and to trigger game actions.

## Repository Structure

```text
.
├── Content/
│   ├── BP_Classes/       # Board, game manager, players, pieces, tiles, HUD, and popup Blueprints
│   ├── Input/            # Enhanced Input action and mapping context assets
│   ├── Materials/        # Board, piece, highlight, and check-state materials
│   ├── PieceImages/      # Piece image assets used by the project
│   └── GameLevel.umap    # Main playable level
├── Config/               # Unreal Engine, input, rendering, and project settings
├── Platforms/            # Platform-specific configuration
├── Source/
│   ├── GameOfChess/
│   │   ├── Public/       # Public C++ headers
│   │   └── Private/      # C++ implementations
│   ├── GameOfChess.Target.cs
│   └── GameOfChessEditor.Target.cs
├── GameOfChess.uproject  # Unreal project descriptor
└── README.md
```

## Requirements

- Unreal Engine **5.2**.
- A desktop installation of the Unreal Engine editor.
- A supported C++ toolchain for the target platform:
  - Visual Studio 2022 with the Desktop development with C++ workload is recommended on Windows.
- Git, if cloning the repository from source control.

## Getting Started

### Clone the repository

```bash
git clone https://github.com/maurovagge/GameOfChess.git
cd GameOfChess
```

### Open the project

1. Open `GameOfChess.uproject` with Unreal Engine 5.2.
2. Allow Unreal Engine to generate project files if prompted.
3. If prompted to build the C++ module, choose **Yes** and wait for compilation to finish.
4. Open the `GameLevel` map if it is not opened automatically.
5. Press **Play** in the Unreal Editor to start a match.

The project configuration sets `GameLevel` as both the editor startup map and the default game map. The Blueprint game manager is configured as the default game mode.

## Controls and Gameplay

The primary interaction is mouse-based:

1. Click a White piece to select it.
2. Click a highlighted destination tile to move it.
3. Wait for the Black AI to complete its turn.

The HUD and initialization popup expose additional actions configured in the Blueprint assets, including opponent selection, restart, score reset, history navigation, undo, and exit. The exact placement of these controls is defined by the current Blueprint UI layout.

During a match, the interface can communicate:

- The active player.
- White and Black scores.
- The latest move.
- Check, checkmate, stalemate, and other game messages.

Move-history playback is intended for reviewing the game state. Normal play is disabled while the board is in playback mode.

## Development Notes

- C++ source files are located in `Source/GameOfChess/Public` and `Source/GameOfChess/Private`.
- Most visual configuration is assigned through Blueprint defaults in `Content/BP_Classes`.
- Piece Blueprint subclasses provide the White/Black visual variants referenced by `ABoard`.
- The main board dimensions are represented by an 8x8 grid with 100 Unreal units per tile.
- The minimax implementation currently uses a fixed search depth of three plies and a material-based evaluation function.
- The project does not currently include a separate automated test suite; gameplay behavior is primarily exercised through the Unreal Editor.

## Building and Packaging

For editor development, opening the `.uproject` file and using Unreal Engine's standard **Build** and **Play** commands is sufficient.

To create a distributable build, use Unreal Editor's **Platforms** menu and select the target desktop platform. Ensure that the C++ module compiles successfully before packaging. Platform-specific settings are stored under `Config` and `Platforms`.

## License

No license file is currently included in this repository. Add a license before distributing or reusing the project if required.
