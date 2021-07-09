#ifndef MIDIINPUT_H
#define MIDIINPUT_H

#include "RtMidi.h"

#include "core/reference.h"


class MidiMessage {
public:
	int _event;
	int _note;
	int _velocity;
	double _time_since_start;

	std::string event_type;
	std::string note_name;

private:
	std::vector<std::string> note_matrix = {
		"C",
		"C#",
		"D",
		"D#",
		"E",
		"F",
		"F#",
		"G",
		"G#",
		"A",
		"A#",
		"B",
	};

public:
	MidiMessage(std::vector<unsigned char> *message, double stamp);

	Array convert_to_array() {
		Array converted;
		converted.push_back(event_type.c_str());
		converted.push_back(note_name.c_str());
		converted.push_back(_velocity);
		converted.push_back(_time_since_start);
		return converted;
	}
};



class MidiInput : public Reference {
	GDCLASS(MidiInput, Reference);


protected:
	static void _bind_methods();


friend void message_callback(double timeStamp, std::vector<unsigned char> *message, void *userData);

private:
	bool is_operating = false;
	String port_name = "null";
	double total_time_since_start;

	RtMidiIn midiin;

public:
	Array get_messages();
	String get_port_name();
	bool is_port_connected(int port);

	void start_input_system(int port, bool use_signals = false);

	MidiInput();
	~MidiInput();
};

void message_callback(double timeStamp, std::vector<unsigned char> *message, void *userData);

#endif // MIDIINPUT_H
