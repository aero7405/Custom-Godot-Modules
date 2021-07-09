#include <iostream>
#include <string>

#include "RtMidi.h"
#include "midi_input.h"
#include "core/object.h"

void message_callback(double timeStamp, std::vector<unsigned char> *message, void *userData) {
	MidiInput *self = static_cast<MidiInput *>(userData);

	if (message->size() < 3) {
		return;
	}

	self->total_time_since_start += timeStamp;
	MidiMessage mes(message, self->total_time_since_start);
	self->emit_signal("midi_action", mes.convert_to_array());
}

void MidiInput::start_input_system(int port, bool use_signals) { // only ever call once
	total_time_since_start = 0;

	// check port
	unsigned int nPorts = midiin.getPortCount();
	if (nPorts < port + 1) {
		std::cout << "Port not available! " << std::endl;

		// disable functionality
		port_name = "null";
		is_operating = false;
	} else { // only should happen if port exists
		port_name = midiin.getPortName(port).c_str();
		is_operating = true;

		midiin.openPort(port);
		std::cout << "Reading MIDI from " << port_name.to_int() << "." << std::endl;
		
		if (use_signals) {
			midiin.setCallback(&message_callback, this);
		}
	}
}

Array MidiInput::get_messages() {
	if (!is_operating) {
		return Array(); // Might be better to return a `null` or something indicating an error.
	}

	Array result;

	while (true) {
		std::vector<unsigned char> message;
		double stamp = midiin.getMessage(&message);

		if (message.size() == 0) {
			break;
		}

		if (message.size() >= 3) {
			total_time_since_start += stamp;

			MidiMessage mes(&message, total_time_since_start);

			result.append(mes.convert_to_array());
		}
	}
	
	return result;
}


String MidiInput::get_port_name() {
	return port_name; // if "null" is returned no port is currently active
}

bool MidiInput::is_port_connected(int port) { // if returns false object needs to be deleted and instanced again - this function should be called at leastt every few seconds
	if (!is_operating) {
		port_name = "null";
		is_operating = false;
		return false;
	}

	unsigned int nPorts = midiin.getPortCount();
	if (nPorts < port + 1) {
		port_name = "null";
		is_operating = false;
		return false;
	}

	return true;
}

// binds methods to class
void MidiInput::_bind_methods() {
	ClassDB::bind_method(D_METHOD("start_input_system", "port", "use_signals"), &MidiInput::start_input_system);

	// requires input system to be started
	ClassDB::bind_method(D_METHOD("get_messages"), &MidiInput::get_messages);
	ClassDB::bind_method(D_METHOD("get_port_name"), &MidiInput::get_port_name);
	ClassDB::bind_method(D_METHOD("is_port_connected", "port"), &MidiInput::is_port_connected);
}


MidiInput::MidiInput() {

}

MidiInput::~MidiInput() {
}


MidiMessage::MidiMessage(std::vector<unsigned char> *message, double stamp) {
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
}
