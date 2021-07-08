#ifndef MIDIINPUT_H
#define MIDIINPUT_H

#include <thread>
#include <memory>

#include "RtMidi.h"

#include "core/object/ref_counted.h"


class Message { // all code is here because it simplifies things in the short term
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
	Message(std::vector<unsigned char> *message, double stamp, bool print_data = true) {
		_time_since_start = stamp;

		_event = (int)message->at(0); // Byte 0 - Event Type
		_note = (int)message->at(1); // Byte 1 - Note
		_velocity = (int)message->at(2); // Byte 2 - Velocity

		if (_event == 144)
			event_type = "note_down";
		else if (_event == 128)
			event_type = "note_up";
		else
			event_type = "null";

		note_name = note_matrix[_note % 12] + std::to_string((_note / 12) - 1);

		if (print_data) {
			std::cout << "Note: " << note_name << "; ";
			std::cout << "Event: " << event_type << "; ";
			std::cout << "Velocity: " << _velocity << "; ";
			std::cout << "Time: " << _time_since_start << "; " << std::endl;
		}
	}

	Array Convert() {
		Array converted;
		converted.push_back(event_type.c_str());
		converted.push_back(note_name.c_str());
		converted.push_back(_velocity);
		converted.push_back(_time_since_start);
		return converted;
	}
};



class MidiInput : public RefCounted {
	GDCLASS(MidiInput, RefCounted);


protected:
	static void _bind_methods();

/*************************************************/
// my crud
public:
	bool is_operating = false;

	String port_name = "null";

	const int MAX_CACHED_MESSAGES = 64;
	std::vector<Message> cached_messages;

	double total_time_since_start;
	std::vector<unsigned char> message;

	RtMidiIn midiin;

	// running variables so we don't initialise theme every frame
	double stamp;

public:
	Array get_messages();
	String get_port_name();
	bool is_port_connected(int port);

	void start_input_system(int port);

	MidiInput();
	~MidiInput();

/*************************************************/

};

void message_callback(double timeStamp, std::vector<unsigned char> *message, void *userData);

#endif // MIDIINPUT_H
