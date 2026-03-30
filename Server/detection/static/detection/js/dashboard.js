(function () {
  const defaults = window.WasteDashboardDefaults || {};

  function readServerConfig() {
    const el = document.getElementById("mqtt-server-config");
    if (!el) {
      return {};
    }

    try {
      return JSON.parse(el.textContent || "{}");
    } catch (_) {
      return {};
    }
  }

  const serverCFG = readServerConfig();
  const mqttDefaults = defaults.mqtt || {};
  const storageKey = defaults.storageKey || "waste-dashboard-mqtt-ws";
  const text = defaults.text || {};

  const inputHost = document.getElementById("host");
  const inputWsPort = document.getElementById("ws-port");
  const inputWsPath = document.getElementById("ws-path");
  const inputCommandTopic = document.getElementById("cmd-topic");
  const inputDataTopic = document.getElementById("data-topic");
  const btnConnect = document.getElementById("btn-connect");
  const btnReconnect = document.getElementById("btn-reconnect");
  const btnDisconnect = document.getElementById("btn-disconnect");
  const portsLine = document.getElementById("ports-line");

  const panelBio = document.getElementById("panel-bio");
  const panelNon = document.getElementById("panel-nonbio");
  const connDot = document.getElementById("conn-dot");
  const connText = document.getElementById("conn-text");
  const topicLine = document.getElementById("topic-line");
  const heroTitle = document.getElementById("hero-title");
  const heroDetail = document.getElementById("hero-detail");
  const lastTime = document.getElementById("last-time");

  if (
    !inputHost ||
    !inputWsPort ||
    !inputWsPath ||
    !inputCommandTopic ||
    !connDot
  ) {
    return;
  }

  let client = null;

  const baseCFG = {
    host: serverCFG.host || mqttDefaults.host || "broker.emqx.io",
    tcpPort: Number(serverCFG.tcpPort || 1883),
    cameraTcpPort: Number(serverCFG.cameraTcpPort || serverCFG.tcpPort || 1883),
    servoTcpPort: Number(serverCFG.servoTcpPort || serverCFG.tcpPort || 1883),
    wsPort: Number(serverCFG.wsPort || mqttDefaults.wsPort || 8084),
    wsPath: serverCFG.wsPath || mqttDefaults.wsPath || "/mqtt",
    wsUseSSL:
      typeof serverCFG.wsUseSSL === "boolean"
        ? serverCFG.wsUseSSL
        : mqttDefaults.wsUseSSL !== false,
    commandTopic:
      serverCFG.commandTopic ||
      mqttDefaults.commandTopic ||
      "waste-management-system/command",
    dataTopic:
      serverCFG.dataTopic ||
      mqttDefaults.dataTopic ||
      "waste-management-system/data",
  };

  const upperBio = new Set(
    (defaults.aliases && defaults.aliases.bio
      ? defaults.aliases.bio
      : ["BIO", "B"]
    ).map(function (v) {
      return String(v).toUpperCase();
    }),
  );

  const upperNonBio = new Set(
    (defaults.aliases && defaults.aliases.nonBio
      ? defaults.aliases.nonBio
      : ["N-BIO", "NB"]
    ).map(function (v) {
      return String(v).toUpperCase();
    }),
  );

  function setConn(ok, msg) {
    connDot.classList.remove("ok", "err");
    connDot.classList.add(ok ? "ok" : "err");
    connText.textContent = msg;
  }

  function saveCFG(cfg) {
    localStorage.setItem(storageKey, JSON.stringify(cfg));
  }

  function loadClientCFG() {
    let saved = {};

    try {
      saved = JSON.parse(localStorage.getItem(storageKey) || "{}");
    } catch (_) {
      saved = {};
    }

    return {
      host: saved.host || baseCFG.host,
      wsPort: Number(saved.wsPort || baseCFG.wsPort),
      wsPath: saved.wsPath || baseCFG.wsPath,
      wsUseSSL:
        typeof saved.wsUseSSL === "boolean" ? saved.wsUseSSL : baseCFG.wsUseSSL,
      commandTopic: saved.commandTopic || baseCFG.commandTopic,
      dataTopic: saved.dataTopic || baseCFG.dataTopic,
    };
  }

  function fillForm(cfg) {
    inputHost.value = cfg.host;
    inputWsPort.value = cfg.wsPort;
    inputWsPath.value = cfg.wsPath;
    inputCommandTopic.value = cfg.commandTopic;
    inputDataTopic.value = cfg.dataTopic;
  }

  function readForm() {
    return {
      host: inputHost.value.trim(),
      wsPort: Number(inputWsPort.value || 0),
      wsPath: inputWsPath.value.trim() || "/mqtt",
      wsUseSSL: baseCFG.wsUseSSL,
      commandTopic: inputCommandTopic.value.trim(),
      dataTopic: inputDataTopic.value.trim(),
    };
  }

  function drawConnectionInfo(cfg) {
    topicLine.textContent =
      "Subscribed: " +
      cfg.commandTopic +
      " + " +
      cfg.dataTopic +
      " | WebSocket: " +
      cfg.host +
      ":" +
      cfg.wsPort +
      cfg.wsPath;

    portsLine.textContent =
      "ESP32 Camera TCP: " +
      baseCFG.host +
      ":" +
      baseCFG.cameraTcpPort +
      " | ESP32 Servo TCP: " +
      baseCFG.host +
      ":" +
      baseCFG.servoTcpPort +
      " | Browser WS: " +
      cfg.host +
      ":" +
      cfg.wsPort +
      cfg.wsPath;
  }

  function showIdle() {
    panelBio.classList.remove("active");
    panelNon.classList.remove("active");
  }

  function normalizePayload(raw) {
    const s = String(raw || "").trim();
    const upper = s.toUpperCase();

    if (upperBio.has(upper)) {
      return "BIO";
    }

    if (upperNonBio.has(upper)) {
      return "N-BIO";
    }

    try {
      const obj = JSON.parse(s);
      const token =
        obj.label ||
        obj.class ||
        obj.type ||
        obj.category ||
        obj.prediction ||
        obj.cmd ||
        "";
      return normalizePayload(token);
    } catch (_) {
      return s;
    }
  }

  function onKind(kind) {
    const ts = new Date().toLocaleTimeString();
    lastTime.textContent = "Last message | " + ts;

    showIdle();

    if (kind === "BIO") {
      panelBio.classList.add("active");
      heroTitle.textContent = text.bioTitle || "Bio waste";
      heroDetail.textContent = text.bioDetail || "Detected as BIO.";
      return;
    }

    if (kind === "N-BIO") {
      panelNon.classList.add("active");
      heroTitle.textContent = text.nonBioTitle || "Non-bio waste";
      heroDetail.textContent = text.nonBioDetail || "Detected as N-BIO.";
      return;
    }

    heroTitle.textContent = text.otherTitle || "Other message";
    heroDetail.textContent = "Received: " + kind;
  }

  function disconnectClient() {
    if (client && client.isConnected()) {
      client.disconnect();
    }

    client = null;
    setConn(false, text.disconnected || "Disconnected");
  }

  function connectClient() {
    const cfg = readForm();

    if (!cfg.host || !cfg.wsPort || !cfg.commandTopic) {
      setConn(false, "Missing host/port/topic");
      heroTitle.textContent =
        text.invalidSettingsTitle || "Invalid MQTT settings";
      heroDetail.textContent =
        text.invalidSettingsDetail ||
        "Host, WS port, and command topic are required.";
      return;
    }

    if (client && client.isConnected()) {
      client.disconnect();
    }

    saveCFG(cfg);
    drawConnectionInfo(cfg);

    const clientId = "webdash_" + Math.random().toString(16).slice(2, 12);
    client = new Paho.MQTT.Client(cfg.host, cfg.wsPort, cfg.wsPath, clientId);

    client.onConnectionLost = function (res) {
      if (res.errorCode !== 0) {
        setConn(false, text.disconnected || "Disconnected");
      }
    };

    client.onMessageArrived = function (message) {
      onKind(normalizePayload(message.payloadString));
    };

    setConn(false, text.connecting || "Connecting...");

    client.connect({
      useSSL: cfg.wsUseSSL,
      timeout: 20,
      mqttVersion: 4,
      onSuccess: function () {
        setConn(true, "Connected | " + cfg.host);
        client.subscribe(cfg.commandTopic);

        if (cfg.dataTopic && cfg.dataTopic !== cfg.commandTopic) {
          client.subscribe(cfg.dataTopic);
        }
      },
      onFailure: function (e) {
        setConn(false, "Connection failed");
        heroTitle.textContent = text.connectFailTitle || "Could not connect";
        heroDetail.textContent =
          (e && e.errorMessage) ||
          text.connectFailDetail ||
          "Check broker settings.";
      },
    });
  }

  btnConnect.addEventListener("click", connectClient);
  btnReconnect.addEventListener("click", function () {
    disconnectClient();
    setTimeout(connectClient, 150);
  });
  btnDisconnect.addEventListener("click", disconnectClient);

  const initialCFG = loadClientCFG();
  fillForm(initialCFG);
  drawConnectionInfo(initialCFG);
  connectClient();
})();
