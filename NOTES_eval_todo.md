# Eval changes to write by hand tomorrow

Reverted today's AI-written versions of these on purpose (round 2 = you write
the engine). Numbers below are what we landed on in conversation — transcribe,
don't copy code, just the shape.

## 1. Closed-four bump — `game_score.c`, `dir_score()`

One open side, run length 4 → score_dir should be **5**, not the formula's
plain `open_sides * length` (which gives 4). Reasoning: a closed four is one
move from a forced win and should outrank an open three (aggregate 18),
so 5 per stone × 4 stones = 20 > 18. Only this one (length, openness) pair
gets special-cased — leave every other combination on the general formula.

```c
if (open_sides == 1 && run_length == 4)
    score_dir = 5;
else
    score_dir = open_sides * run_length;
```

## 2. Capture term — `game_eval.c`, `evaluate()`

Quadratic in captured stones, added to `score[0] - score[1]` before the
turn-parity flip:

- weight chosen so **8 captured stones ≈ 40** (just above open_four's 32).
  weight = 40 / 8² = 0.625.
- ⚠️ tonight I said "scale to 40 at 80 stones" — that's almost certainly a
  typo for **8**, since `MAX_CAPTURES` is 16 and capture-victory triggers at
  10. Recheck against this note before typing it in; don't transcribe "80"
  literally.
- Use float internally for the quadratic term so small early values (2-4
  captured stones) don't get flattened to 0 by integer truncation. Round
  once at the very end, keep `evaluate()`'s return type `int`.

## 3. Terminal capture-win short-circuit — same function

When captures[player] hits **10 stones** (= 5 pairs — captures[] counts
stones, not pairs, per the existing convention), evaluate() should jump to
**±100000**, well above the five-in-a-row cap (10000/50000), overriding the
quadratic term entirely rather than being computed by it. This is an
evaluator-only signal — it does NOT replace the actual capture-victory rule
in `has_won()`, which is still Samu's open task (Aug 21).

## 4. Board-position term — not designed yet, just a marker

Third evaluator: reward central stones over edge ones. Working name was
"manhattan distance from center" but flagged in conversation as probably
wrong — you want something like the *inverse* of distance (closer = higher),
and plain Manhattan distance may not be the right shape either (diagonals?
squared distance falls off faster). Needs actual thought before coding, not
a copy-paste. Whatever the formula ends up being:

- Precompute it into a `static const int center_weight[BOARD_CELLS]` table,
  filled once (e.g. in `initialize_game_state` or as a literal), so
  `evaluate()` just does one array read per stone — no per-call division or
  modulo. This was the whole point of preferring a table over live
  arithmetic in the hot path.

## TODO roadmap

### 5. Score candidate moves from the whole chain

`check_ray_score()` currently scores an empty move from the adjacent stone's
value. Change it to use the value of the chain reached in that direction,
using the chain's start/length metadata, while retaining the direction and
intersection multipliers. This lets a four-stone chain outweigh a three-stone
chain when the candidate limit is small.

### 6. Add Zobrist hashing and a transposition table

- Hash the board, side to move, and capture counts.
- Update the hash incrementally on play, capture, and undo.
- Store depth, score, bound type, and best move in each table entry.
- Reuse the stored best move for move ordering.

### 7. Implement terminal conditions

Implement `has_won()` / `is_terminal()` for:

- five stones in a row;
- capture victory at the agreed capture count;
- no legal moves, if the rules require a full-board draw/terminal state.

Terminal scores must dominate ordinary chain and capture evaluation scores.

### 8. Add captured stones to evaluation

Include the capture-count term in `evaluate()` with the correct player
perspective. Keep the capture-victory check separate from the ordinary
capture bonus, and verify that undo restores both capture counts and the
evaluation exactly.

### 9. Validate selective search

With the candidate limit set low, add positions covering immediate wins,
mandatory blocks, captures, and double threats. Confirm that every mandatory
move remains inside the retained candidate set before relying on the limit
for strength.
