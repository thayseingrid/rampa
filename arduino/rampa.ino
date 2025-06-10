const int numLDRs = 4;
const int ldrPins[numLDRs] = {A0, A1, A2, A3};
const int laserPins[numLDRs] = {12, 11, 10, 9};

unsigned long tempoEscuro[numLDRs];
bool detectado[numLDRs];
int comLaser[numLDRs];
int semLaser[numLDRs];

bool todosDetectados = false;
bool enviado = false;
bool capturarDados = false;

void setup() {
    Serial.begin(9600);

    resetarTudo();

    for (int i = 0; i < numLDRs; ++i) {
        pinMode(laserPins[i], OUTPUT);
    }
}

void loop() {
    lerConfiguracaoSerial();
    lerSensores();
    enviarDados();
}

void lerConfiguracaoSerial() {
    static String entradaSerial = "";

    while (Serial.available() > 0) {
        char c = Serial.read();

        if (c == '\n' || c == '\r') {
            entradaSerial.trim();    // Remove espaços em branco extras

            if (entradaSerial.equalsIgnoreCase("CLASER")) {
                configurarSensoresComLaser();
            } else if (entradaSerial.equalsIgnoreCase("SLASER")) {
                configurarSensoresSemLaser();
            } else if (entradaSerial.equalsIgnoreCase("LASER")) {
                configurarLasers();
            } else if (entradaSerial.equalsIgnoreCase("RESET")) {
                resetarTudo();
            } else if (entradaSerial.equalsIgnoreCase("CAPTURAR")) {
                capturarDados = true;
                resetarDeteccao();
                resetarTempos();
                Serial.println("Iniciando captura de dados...");
            }

            entradaSerial = "";
        } else {
            entradaSerial += c;
        }
    }
}

void resetarTempos() {
    for (int i = 0; i < numLDRs; i++) {
        tempoEscuro[i] = 0;
    }
}

void resetarDeteccao() {
    for (int i = 0; i < numLDRs; i++) {
        detectado[i] = false;
    }
}

void resetarTudo() {
    enviado = false;
    capturarDados = false;
    resetarTempos();
    resetarDeteccao();
}

bool sensorAtivou(int n) {
    int leitura = analogRead(n);
    int v0 = abs(leitura - comLaser[n]);
    int v1 = abs(leitura - semLaser[n]);

    if (v1 < v0) {
        return true;
    } else {
        return false;
    }
}

void lerSensores() {
    if (!capturarDados) {
        return;
    }

    for (int i = 0; i < numLDRs; i++) {
        if (!detectado[i] && sensorAtivou(i)) {
            tempoEscuro[i] = micros();
            detectado[i] = true;
        }
    }
}

void configurarLasers() {
    for (int i = 0; i < numLDRs; ++i) {
        digitalWrite(laserPins[i], LOW);
    }

    delay(500);
    configurarSensoresSemLaser();

    for (int i = 0; i < numLDRs; ++i) {
        digitalWrite(laserPins[i], HIGH);
    }

    delay(500);
    configurarSensoresComLaser();
}

void configurarSensoresSemLaser() {
    for (int i = 0; i < numLDRs; i++) {
        int acc = 0;

        for (int j = 0; j < 10; ++j) {
            int leitura = analogRead(ldrPins[i]);
            acc += leitura;
            delayMicroseconds(1000);
        }
        
        semLaser[i] = acc / 10;

        Serial.print("sem laser = ");
        Serial.println(semLaser[i]);
    }

    Serial.println("====");
}

void configurarSensoresComLaser() {
    for (int i = 0; i < numLDRs; i++) {
        int acc = 0;

        for (int j = 0; j < 10; ++j) {
            int leitura = analogRead(ldrPins[i]);
            acc += leitura;
            delayMicroseconds(1000);
        }
        
        comLaser[i] = acc / 10;

        Serial.print("com laser = ");
        Serial.println(comLaser[i]);
    }

    Serial.println("====");
}

void enviarDados() {
    if (enviado) {
        return;
    }

    for (int i = 0; i < numLDRs; ++i) {
        if (!detectado[i]) {
            Serial.println("NODATA");
            return;
        }
    }

    Serial.print("GET ");

    for (int i = 0; i < numLDRs; ++i) {
        if (i > 0) {
            Serial.print(" ");
        }

        Serial.print(tempoEscuro[i]);
    }

    Serial.println();
    enviado = true;
    capturarDados = false;
}