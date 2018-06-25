#include "utils.h"
#include <iostream>

using namespace std;

namespace ce30_driver {
bool GetVersion(std::string& version, UDPSocket& socket) {
  VersionRequestPacket version_request;
  auto diagnose = socket.SendPacket(version_request);
  if (diagnose == Diagnose::send_successful) {
    VersionResponsePacket version_response;
    diagnose = socket.GetPacket(version_response);
    if (diagnose == Diagnose::receive_successful) {
      version = version_response.GetVersionString();
      return true;
    } else {
      cerr << "'Get Version' not Responding" << endl;
    }
  } else {
    cerr << "Request 'Get Version' Failed" << endl;
  }
  return false;
}

bool GetDeviceID(int& id, UDPSocket& socket) {
  GetIDRequestPacket get_id_request;
  auto diagnose = socket.SendPacket(get_id_request);
  if (diagnose == Diagnose::send_successful) {
    GetIDResponsePacket get_id_response;
    diagnose = socket.GetPacket(get_id_response);
    if (diagnose == Diagnose::receive_successful) {
      id = get_id_response.ID();
      return true;
    } else {
      cerr << "'Get ID' not Responding" << endl;
    }
  } else {
    cerr << "Request 'Get ID' Failed" << endl;
  }
  return false;
}

bool SetDeviceID(const int& id, UDPSocket& socket) {
  SetIDRequestPacket set_id_request(id);
  auto diagnose = socket.SendPacket(set_id_request);
  if (diagnose == Diagnose::send_successful) {
    CommonResponsePacket set_id_response;
    diagnose = socket.GetPacket(set_id_response);
    if (diagnose == Diagnose::receive_successful) {
      if (set_id_response.Successful()) {
        return true;
      } else {
        cerr << "'Set ID' Failed" << endl;
      }
    } else {
      cerr << "'Set ID' not Responding" << endl;
    }
  } else {
    cerr << "Request 'Set ID' Failed" << endl;
  }
  return false;
}

bool StartRunning(UDPSocket& socket) {
  StartRequestPacket start_request;
  auto diagnose = socket.SendPacket(start_request);
  if (diagnose == Diagnose::send_successful) {
    return true;
  } else {
    cerr << "Request 'Start' Failed" << endl;
  }
  return false;
}

bool StopRunning(UDPSocket& socket) {
  StopRequestPacket stop_request;
  auto diagnose = socket.SendPacket(stop_request);
  if (diagnose == Diagnose::send_successful) {
    Packet packet;
    diagnose = socket.GetPacket(packet);
    if (diagnose == Diagnose::receive_successful) {
      // cerr << "Still Receving Packets though Stop Packet was Sent" << endl;
      return false;
    } else {
      return true;
    }
  } else {
    cerr << "Request 'Stop' Failed" << endl;
  }
  return false;
}

/// @cond DO_NOT_DOCUMENT_THIS
bool EnableFilter(UDPSocket& socket) {
  EnableFilterRequestPacket request_packet;
  auto diagnose = socket.SendPacket(request_packet);
  if (diagnose == Diagnose::send_successful) {
    EnableFilterResponsePacket response_packet;
    diagnose = socket.GetPacket(response_packet);
    if (diagnose == Diagnose::receive_successful) {
      if (response_packet.Successful()) {
        return true;
      } else {
        cerr << "'Enable Filter' Failed" << endl;
      }
    } else {
      cerr << "'Enable Filter' not Responding" << endl;
    }
  } else {
    cerr << "Request 'Enable Filter' Failed" << endl;
  }
  return false;
}
/// @endcond

/// @cond DO_NOT_DOCUMENT_THIS
bool DisableFilter(UDPSocket& socket) {
  DisableFilterRequestPacket request_packet;
  auto diagnose = socket.SendPacket(request_packet);
  if (diagnose == Diagnose::send_successful) {
    DisableFilterResponsePacket response_packet;
    diagnose = socket.GetPacket(response_packet);
    if (diagnose == Diagnose::receive_successful) {
      if (response_packet.Successful()) {
        return true;
      } else {
        cerr << "'Disable Filter' Failed" << endl;
      }
    } else {
      cerr << "'Disable Filter' not Responding" << endl;
    }
  } else {
    cerr << "Request 'Disable Filter' Failed" << endl;
  }
  return false;
}
/// @endcond

bool Connect(UDPSocket& socket) {
  return socket.Connect() == Diagnose::connect_successful;
}

bool GetPacket(
    PacketBase& packet, UDPSocket& socket, const bool& thread_safe) {
  Diagnose diagnose;
  if (thread_safe) {
    diagnose = socket.GetPacketThreadSafe(packet);
  } else {
    diagnose = socket.GetPacket(packet);
  }
  return diagnose == Diagnose::receive_successful;
}

bool SendPacket(
    const PacketBase &packet, UDPSocket &socket, const bool &thread_safe) {
  Diagnose diagnose;
  if (thread_safe) {
    diagnose = socket.SendPacketThreadSafe(packet);
  } else {
    diagnose = socket.SendPacket(packet);
  }
  return diagnose == Diagnose::send_successful;
}
}
