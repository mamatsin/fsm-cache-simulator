# fsm-cache-simulator
# Cache Controller FSM Simulator

## 📌 Description
This project simulates a Finite State Machine (FSM) based cache controller as described in *Computer Architecture and Design (Patterson & Hennessy, RISC-V Edition)*.

The simulator models cache behavior including:
- Cache hits and misses
- Write-back policy
- Allocate on miss
- Memory latency

---

## ⚙️ Features
- Direct-mapped cache
- FSM states:
  - IDLE
  - COMPARE_TAG
  - WRITE_BACK
  - ALLOCATE
- Simulated memory with latency
- CPU request queue

---

## 🛠️ Requirements
- C++ compiler (g++ recommended)

---

## ▶️ How to Build

```bash
g++ main.cpp -o cache_sim
