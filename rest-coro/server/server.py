# Run:
# C:\Users\AlexanderKatz\Projects\CPP\rest-coro\server\venv\Scripts\python.exe C:\Users\AlexanderKatz\Projects\CPP\rest-coro\server\server.py

from flask import Flask, request, jsonify
from datetime import datetime

app = Flask(__name__)

# Sample data
people_data = [
    {"firstName": "John", "lastName": "Doe", "dateOfBirth": "1980-01-01"},
    {"firstName": "Jane", "lastName": "Smith", "dateOfBirth": "1990-02-02"},
    {"firstName": "Alice", "lastName": "Johnson", "dateOfBirth": "1975-03-03"},
]


@app.route('/v1/people', methods=['GET'])
def get_people():
    datetime_str = request.args.get('datetime')

    # Validate datetime format
    try:
        datetime_obj = datetime.fromisoformat(datetime_str)
    except ValueError:
        return jsonify({"error": "Invalid date-time format"}), 400

    # Here you could filter `people_data` based on `datetime_obj`
    # For simplicity, returning all the data
    return jsonify(people_data), 200


if __name__ == '__main__':
    app.run(host='172.29.208.1', port=5000)
