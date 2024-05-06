from flask import Flask, request, jsonify
import sqlite3

app = Flask(__name__)

def init_db():
    conn = sqlite3.connect('sensor_data.db')
    c = conn.cursor()
    c.execute('''
        CREATE TABLE IF NOT EXISTS sensor_logs (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            value INTEGER,
            detected_anomaly BOOLEAN
        )
    ''')
    conn.commit()
    conn.close()

@app.route('/sensor-data', methods=['POST'])
def receive_data():
    value = int(request.form['value'])
    anomaly_detected = value > 75

    conn = sqlite3.connect('sensor_data.db')
    c = conn.cursor()
    c.execute('INSERT INTO sensor_logs (value, detected_anomaly) VALUES (?, ?)', (value, anomaly_detected))
    conn.commit()
    conn.close()

    if anomaly_detected:
        print(f"Anomaly detected! Value: {value}")
        # Placeholder for additional alert logic
    
    return jsonify({"status": "Success", "msg": "Data received and logged"}), 200

if __name__ == '__main__':
    init_db()
    app.run(debug=True, port=5000, ssl_context='adhoc')  # Uses ad-hoc SSL for HTTPS
