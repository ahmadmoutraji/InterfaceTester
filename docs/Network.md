Network IP selection

Primary interface logic (deterministic):
- Consider interfaces that are Up and Running.
- Exclude loopback interfaces.
- Sort by human-readable interface name (case-insensitive).
- Use the first IPv4 address found.

If no qualifying IPv4 address exists, display "Not Available".

Update strategy
- Poll every 2 seconds using QNetworkInterface to refresh the primary IP.
