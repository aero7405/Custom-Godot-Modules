#include "register_types.h"

#include "core/class_db.h"
#include "midi_input.h"

void register_midi_input_types() {
	ClassDB::register_class<MidiInput>();
}

void unregister_midi_input_types() {
	// Nothing to do here in this example.
}
