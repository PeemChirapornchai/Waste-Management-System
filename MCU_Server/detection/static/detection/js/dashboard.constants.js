window.WasteDashboardDefaults = {
  storageKey: "waste-dashboard-mqtt-ws",
  mqtt: {
    host: "broker.emqx.io",
    wsPort: 8084,
    wsPath: "/mqtt",
    wsUseSSL: true,
    commandTopic: "waste-management-system/command",
    dataTopic: "waste-management-system/data",
  },
  aliases: {
    bio: ["BIO", "B", "BIODEGRADABLE"],
    nonBio: ["N-BIO", "NON-BIO", "NON_BIO", "NB", "NONBIO", "NONBIODEGRADABLE"],
  },
  text: {
    connecting: "Connecting...",
    disconnected: "Disconnected",
    invalidSettingsTitle: "Invalid MQTT settings",
    invalidSettingsDetail: "Host, WS port, and command topic are required.",
    connectFailTitle: "Could not connect",
    connectFailDetail: "Check broker, WS port/path, and firewall.",
    bioTitle: "Bio waste",
    bioDetail: "Detected as b / BIO. Servo route goes to biodegradable side.",
    nonBioTitle: "Non-bio waste",
    nonBioDetail:
      "Detected as non-b / N-BIO. Servo route goes to non-biodegradable side.",
    otherTitle: "Other message",
  },
};
