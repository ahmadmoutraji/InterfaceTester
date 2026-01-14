Logging

Location
- Primary: /var/log/interface-tester/
- Fallback (if not writable): QStandardPaths::AppDataLocation + "/logs"

Rotation
- Daily log files named interface-tester-YYYYMMDD.log
- Old logs are removed on startup; retention is 7 days

Content
- Each line includes ISO 8601 timestamp, test name, and message
