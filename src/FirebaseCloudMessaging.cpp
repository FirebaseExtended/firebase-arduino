#include "FirebaseCloudMessaging.h"

FirebaseCloudMessage FirebaseCloudMessage::SimpleNotification(
    const std::string& title, const std::string& body) {
  FirebaseCloudMessage message;
  message.notification.title = title;
  message.notification.body = body;
  return message;
}

FirebaseCloudMessaging::FirebaseCloudMessaging(const std::string& server_key) {
  auth_header_ = "key=";
  auth_header_ += server_key;
}

const FirebaseError FirebaseCloudMessaging::SendMessageToUser(
    const std::string& registration_id,
    const FirebaseCloudMessage& message) {
  DynamicJsonBuffer buffer;
  JsonObject& root = buffer.createObject();
  root["to"] = registration_id.c_str();

  AddToJson(message, root);

  char payload[root.measureLength() + 1];
  root.printTo(payload, sizeof(payload));
  return SendPayload(payload);
}

const FirebaseError FirebaseCloudMessaging::SendMessageToUsers(
    const std::vector<std::string>& registration_ids,
    const FirebaseCloudMessage& message) {
  DynamicJsonBuffer buffer;
  JsonObject& root = buffer.createObject();
  JsonArray& ids = root.createNestedArray("registration_ids");
  for (const std::string& id : registration_ids) {
    ids.add(id.c_str());
  }

  AddToJson(message, root);

  char payload[root.measureLength() + 1];
  root.printTo(payload, sizeof(payload));
  return SendPayload(payload);
}


const FirebaseError FirebaseCloudMessaging::SendMessageToTopic(
    const std::string& topic, const FirebaseCloudMessage& message) {
  std::string to("/topics/");
  to += topic;

  DynamicJsonBuffer buffer;
  JsonObject& root = buffer.createObject();
  root["to"] = to.c_str();

  AddToJson(message, root);

  char payload[root.measureLength() + 1];
  root.printTo(payload, sizeof(payload));
  return SendPayload(payload);
}

const FirebaseError FirebaseCloudMessaging::SendPayload(
    const char* payload) {
  std::shared_ptr<FirebaseHttpClient> client(FirebaseHttpClient::create());
  client->begin("http://fcm.googleapis.com/fcm/send");
  client->addHeader("Authorization", auth_header_.c_str());
  client->addHeader("Content-Type", "application/json");

  int status = client->sendRequest("POST", payload);
  if (status != 200) {
    return FirebaseError(status, client->errorToString(status));
  } else {
    return FirebaseError::OK();
  }
}

const void FirebaseCloudMessaging::AddToJson(
    const FirebaseCloudMessage& message, JsonObject& json) const {
  if (!message.collapse_key.empty()) {
    json["collapse_key"] = message.collapse_key.c_str();
  }

  json["priority"] = message.high_priority ? "high" : "normal";
  json["delay_while_idle"] = message.delay_while_idle;
  if (message.time_to_live > 0 && message.time_to_live < 2419200) {
    json["time_to_live"] = message.time_to_live;
  }

  if (!message.data.empty()) {
    JsonObject& data = json.createNestedObject("data");
    for (const auto& datum : message.data) {
      data[datum.first.c_str()] = datum.second.c_str();
    }
  }

  if (!message.notification.title.empty() || !message.notification.body.empty()) {
    JsonObject& notification = json.createNestedObject("notification");
    if (!message.notification.title.empty()) {
      notification["title"] = message.notification.title.c_str();
    }
    if (!message.notification.body.empty()) {
      notification["body"] = message.notification.body.c_str();
    }
  }
}

