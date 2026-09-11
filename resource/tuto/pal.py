import json
import string


# ============================================================
# CONFIGURATION
# ============================================================

OUTPUT_FILE = "is_palindrome.json"

BLANK = "_"
MARK = "#"

YES = "y"
NO = "n"

UPPERCASE = string.ascii_uppercase
LOWERCASE = string.ascii_lowercase
DIGITS = string.digits

# Input characters.
#
# Spaces are accepted and ignored.
INPUT_CHARACTERS = list(
	UPPERCASE +
	LOWERCASE +
	DIGITS +
	" "
)

# Characters that are compared.
#
# A/a -> a
# B/b -> b
# ...
# Z/z -> z
# 0-9 -> themselves
COMPARE_CHARS = list(
	LOWERCASE +
	DIGITS
)

# The complete tape alphabet.
#
# _ = blank
# # = left/right limit and processed character
TAPE_ALPHABET = (
	INPUT_CHARACTERS +
	[BLANK, MARK]
)


# Remove duplicates while preserving order.
ALPHABET = list(dict.fromkeys(TAPE_ALPHABET))


# ============================================================
# CHARACTER HELPERS
# ============================================================

def canonical(character):
	return character.lower()


def matches(character, remembered):
	return canonical(character) == remembered


# ============================================================
# STATE HELPERS
# ============================================================

def continue_state(character):
	return f"continue_to_end_{character}"


def if_state(character):
	return f"if_{character}"


# ============================================================
# TRANSITION HELPER
# ============================================================

def make_transition(read, to_state, write, action):
	return {
		"read": read,
		"to_state": to_state,
		"write": write,
		"action": action
	}


# ============================================================
# STATES
# ============================================================

states = [
	"start"
]

# One state for each possible remembered character.
for character in COMPARE_CHARS:
	states.append(
		continue_state(character)
	)

# One comparison state for each possible character.
for character in COMPARE_CHARS:
	states.append(
		if_state(character)
	)

states.extend([
	"backward_until_left_limit",
	"prepare_next",
	"write_yes",
	"write_no",
	"END"
])


# ============================================================
# TRANSITIONS
# ============================================================

transitions = {}


# ============================================================
# START
#
# Find the first unprocessed character.
#
# Spaces are ignored.
#
# Example:
#
#	abbba
#
# becomes:
#
#	#bbba
#
# The first character is remembered as 'a'.
# ============================================================

transitions["start"] = []

# Ignore spaces.
transitions["start"].append(
	make_transition(
		read=" ",
		to_state="start",
		write=" ",
		action="RIGHT"
	)
)

# Find the first unprocessed character.
for character in INPUT_CHARACTERS:

	if character == " ":
		continue

	remembered = canonical(character)

	transitions["start"].append(
		make_transition(
			read=character,
			to_state=continue_state(remembered),
			write=MARK,
			action="RIGHT"
		)
	)


# ============================================================
# CONTINUE TO END
#
# We have already marked the left character with #.
#
# Move RIGHT until the right limit.
#
# The first time we reach the end:
#
#	#bbba_
#
# write # on the blank:
#
#	#bbba#
#
# and move LEFT into if_<c>.
#
# On later iterations the right # already exists, so:
#
#	read # -> if_<c>, #, LEFT
#
# This is the important boundary transition.
# ============================================================

for remembered in COMPARE_CHARS:

	state = continue_state(remembered)

	transitions[state] = []

	# Move right over ordinary characters.
	for character in INPUT_CHARACTERS:

		transitions[state].append(
			make_transition(
				read=character,
				to_state=state,
				write=character,
				action="RIGHT"
			)
		)

	# The right # is the limit.
	#
	# Do NOT continue right.
	#
	# Go directly to if_<c> and move left.
	transitions[state].append(
		make_transition(
			read=MARK,
			to_state=if_state(remembered),
			write=MARK,
			action="LEFT"
		)
	)

	# First iteration reaches the blank at the end.
	#
	# Turn the blank into the right #.
	transitions[state].append(
		make_transition(
			read=BLANK,
			to_state=if_state(remembered),
			write=BLANK,
			action="LEFT"
		)
	)


# ============================================================
# IF_<CHARACTER>
#
# We are looking for the rightmost unprocessed character.
#
# Example:
#
#	#bbba#
#	   ^
#
# For if_a, the 'a' matches:
#
#	#bbb##
#
# Then move LEFT.
#
# Spaces are ignored.
#
# If we reach # before finding an ordinary character,
# there is no character left to compare.
#
# That means the remaining character was the middle character,
# so the input is a palindrome.
# ============================================================

for remembered in COMPARE_CHARS:

	state = if_state(remembered)

	transitions[state] = []

	# Ignore spaces while moving left.
	transitions[state].append(
		make_transition(
			read=" ",
			to_state=state,
			write=" ",
			action="LEFT"
		)
	)

	# If we encounter # before finding a character,
	# there is no unprocessed character remaining.
	#
	# This is the odd-length / single-character case.
	transitions[state].append(
		make_transition(
			read=MARK,
			to_state="write_yes",
			write=MARK,
			action="RIGHT"
		)
	)

	# Compare every possible input character.
	for character in INPUT_CHARACTERS:

		if character == " ":
			continue

		if matches(character, remembered):

			# Match:
			#
			#	#bbb a#
			#
			# becomes:
			#
			#	#bbb##
			#
			# and move left.
			transitions[state].append(
				make_transition(
					read=character,
					to_state="backward_until_left_limit",
					write=MARK,
					action="LEFT"
				)
			)

		else:

			# Mismatch -> reject.
			transitions[state].append(
				make_transition(
					read=character,
					to_state="write_no",
					write=MARK,
					action="RIGHT"
				)
			)


# ============================================================
# BACKWARD UNTIL LEFT LIMIT
#
# After matching the right character, go LEFT until the
# left-side #.
#
# Example:
#
#	#bbb##
#	   ^
#
# Move left until:
#
#	#bbb##
#	^
#
# Then move RIGHT into prepare_next.
# ============================================================

transitions["backward_until_left_limit"] = []

# Move left over ordinary characters.
for character in INPUT_CHARACTERS:

	transitions["backward_until_left_limit"].append(
		make_transition(
			read=character,
			to_state="backward_until_left_limit",
			write=character,
			action="LEFT"
		)
	)

# Spaces are already included above, but this explicit
# transition documents that they are ignored.
#
# It is not added twice because the transition dictionary
# would otherwise contain duplicate reads.
#
# The # encountered here is the left limit.
transitions["backward_until_left_limit"].append(
	make_transition(
		read=MARK,
		to_state="prepare_next",
		write=MARK,
		action="RIGHT"
	)
)

# Reaching blank here means malformed input.
transitions["backward_until_left_limit"].append(
	make_transition(
		read=BLANK,
		to_state="END",
		write=BLANK,
		action="LEFT"
	)
)


# ============================================================
# PREPARE NEXT
#
# We are immediately to the RIGHT of the left #.
#
# Skip:
#
#	#
#	#
#	spaces
#
# until the next unprocessed character.
#
# Example:
#
#	#bbb##
#	 ^
#
# Mark the next character:
#
#	##bb##
#
# and remember it.
# ============================================================

transitions["prepare_next"] = []

# Skip spaces.
transitions["prepare_next"].append(
	make_transition(
		read=" ",
		to_state="prepare_next",
		write=" ",
		action="RIGHT"
	)
)

# Skip processed / left-limit # characters.
transitions["prepare_next"].append(
	make_transition(
		read=MARK,
		to_state="prepare_next",
		write=MARK,
		action="RIGHT"
	)
)

# Find the next unprocessed character.
for character in INPUT_CHARACTERS:

	if character == " ":
		continue

	remembered = canonical(character)

	transitions["prepare_next"].append(
		make_transition(
			read=character,
			to_state=continue_state(remembered),
			write=MARK,
			action="RIGHT"
		)
	)

# Nothing remains.
#
# Everything between the limits is # or spaces.
transitions["prepare_next"].append(
	make_transition(
		read=BLANK,
		to_state="write_yes",
		write=BLANK,
		action="RIGHT"
	)
)


# ============================================================
# WRITE YES
#
# Move RIGHT to the blank after the right # and write y.
# ============================================================

transitions["write_yes"] = []

for character in ALPHABET:

	if character == BLANK:
		continue

	transitions["write_yes"].append(
		make_transition(
			read=character,
			to_state="write_yes",
			write=character,
			action="RIGHT"
		)
	)

# Write the result at the first blank.
transitions["write_yes"].append(
	make_transition(
		read=BLANK,
		to_state="END",
		write=YES,
		action="LEFT"
	)
)


# ============================================================
# WRITE NO
#
# Move RIGHT to the blank after the right # and write n.
# ============================================================

transitions["write_no"] = []

for character in ALPHABET:

	if character == BLANK:
		continue

	transitions["write_no"].append(
		make_transition(
			read=character,
			to_state="write_no",
			write=character,
			action="RIGHT"
		)
	)

# Write the result at the first blank.
transitions["write_no"].append(
	make_transition(
		read=BLANK,
		to_state="END",
		write=NO,
		action="LEFT"
	)
)


# ============================================================
# FINAL STATES
#
# END is deliberately NOT put in transitions.
# It is a terminal state.
# ============================================================


# ============================================================
# MACHINE
# ============================================================

machine = {
	"name": "is_palindrome",
	"alphabet": ALPHABET,
	"blank": BLANK,
	"states": states,
	"initial": "start",
	"finals": [
		"END"
	],
	"transitions": transitions
}


# ============================================================
# COMPACT JSON FORMATTER
#
# Output style:
#
#	{
#		"name": "is_palindrome",
#		"alphabet": ["A", "B", "C", ...],
#		"blank": "_",
#		"states": ["start", ...],
#		"initial": "start",
#		"finals": ["END"],
#		"transitions": {
#			"start": [
#				{ "read": " ", "to_state": "start", "write": " ", "action": "RIGHT" },
#				{ "read": "A", "to_state": "continue_to_end_a", "write": "#", "action": "RIGHT" }
#			]
#		}
#	}
#
# Indentation uses actual tabs.
# ============================================================

def format_json(obj, level=0):

	indent = "\t" * level
	next_indent = "\t" * (level + 1)

	# --------------------------------------------------------
	# Dictionary
	# --------------------------------------------------------

	if isinstance(obj, dict):

		if not obj:
			return "{}"

		lines = ["{"]

		items = list(obj.items())

		for index, (key, value) in enumerate(items):

			comma = "," if index < len(items) - 1 else ""

			key_text = json.dumps(
				key,
				ensure_ascii=False
			)

			value_text = format_json(
				value,
				level + 1
			)

			lines.append(
				f'{next_indent}{key_text}: {value_text}{comma}'
			)

		lines.append(indent + "}")

		return "\n".join(lines)

	# --------------------------------------------------------
	# List
	# --------------------------------------------------------

	if isinstance(obj, list):

		if not obj:
			return "[]"

		# Simple lists stay on one line.
		#
		# ["A", "B", "C"]
		#
		if all(
			not isinstance(item, (dict, list))
			for item in obj
		):

			return json.dumps(
				obj,
				ensure_ascii=False
			)

		lines = ["["]

		for index, item in enumerate(obj):

			comma = "," if index < len(obj) - 1 else ""

			# Transition objects stay on one line.
			if isinstance(item, dict):

				item_text = json.dumps(
					item,
					ensure_ascii=False,
					separators=(", ", ": ")
				)

				lines.append(
					f"{next_indent}{item_text}{comma}"
				)

			else:

				item_text = format_json(
					item,
					level + 1
				)

				lines.append(
					f"{next_indent}{item_text}{comma}"
				)

		lines.append(indent + "]")

		return "\n".join(lines)

	# --------------------------------------------------------
	# Simple values
	# --------------------------------------------------------

	return json.dumps(
		obj,
		ensure_ascii=False
	)


# ============================================================
# WRITE JSON
# ============================================================

with open(
	OUTPUT_FILE,
	"w",
	encoding="utf-8"
) as file:

	file.write(
		format_json(machine)
	)

	file.write("\n")


# ============================================================
# REPORT
# ============================================================

print("=" * 60)
print("Turing Machine generated successfully")
print("=" * 60)
print()
print(f"Output file: {OUTPUT_FILE}")
print()
print("Input:")
print("  Uppercase letters : A-Z")
print("  Lowercase letters : a-z")
print("  Digits            : 0-9")
print("  Spaces            : ignored")
print()
print("Tape alphabet:")
print("  Input characters + '_' + '#'")
print()
print("Comparison:")
print("  Case-insensitive")
print("  A == a")
print("  B == b")
print("  ...")
print("  Z == z")
print()
print("Boundary:")
print("  '#' is used as both left and right limit")
print()
print("Results:")
print("  y = palindrome")
print("  n = not palindrome")
print()
print(f"Input alphabet size : {len(INPUT_CHARACTERS)}")
print(f"Tape alphabet size  : {len(ALPHABET)}")
print(f"Comparison states   : {len(COMPARE_CHARS)}")
print(f"Total states        : {len(states)}")
print("=" * 60)
