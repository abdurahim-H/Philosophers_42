<h1 align="center">🍽️ Philosophers</h1>

<p align="center">
  <img src="https://i.imgur.com/8kRWYr2.png" alt="Philosophers Banner">
</p>

<p align="center">
  <img src="https://img.shields.io/badge/language-C-blue.svg" alt="Language">
  <img src="https://img.shields.io/badge/license-MIT-green.svg" alt="License">
  <img src="https://img.shields.io/badge/version-1.0.0-red.svg" alt="Version">
</p>

## 🌟 Introduction

Welcome to the **Philosophers** project! This simulation brings the classic dining philosophers problem to life, challenging you to navigate the delicate balance between hungry philosophers, limited forks, and the ever-present threat of deadlock.

## 🍝 The Problem

Imagine a group of philosophers seated around a circular table, with a large bowl of spaghetti in the center. There are as many forks as philosophers, placed between each pair. A philosopher needs two forks to eat, but can only use the forks to their immediate left and right.

The challenge? Ensure that everyone gets to eat without causing a deadlock!

## 🛠️ Features

- 🧠 Simulates multiple philosophers thinking and eating
- 🍴 Implements mutex-based fork management
- ⏱️ Precise timing for eating, sleeping, and thinking
- 💀 Death detection if a philosopher doesn't eat in time
- 🔄 Customizable number of philosophers and meal times

## 🚀 Getting Started

### Prerequisites

- GCC compiler
- Make utility
- UNIX-based operating system (Linux/MacOS)

### Compilation

```bash
make
```

### Running the Simulation

```bash
./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]
```

Example:
```bash
./philo 5 800 200 200 7
```

## 🎨 Output

The simulation provides a console output in the following format:

```
timestamp_in_ms X has taken a fork
timestamp_in_ms X is eating
timestamp_in_ms X is sleeping
timestamp_in_ms X is thinking
timestamp_in_ms X died
```

Where X is the philosopher number.

## 🧪 Testing Scenarios

1. **Basic Test**: `./philo 5 800 200 200`
   - Should run indefinitely with no deaths

2. **Death Scenario**: `./philo 4 310 200 100`
   - A philosopher should die

3. **Optional Argument**: `./philo 5 800 200 200 7`
   - Should stop after each philosopher eats 7 times

4. **Edge Cases**:
   - `./philo 1 800 200 200` (One philosopher)
   - `./philo 4 410 200 200` (Just in time)
   - `./philo 200 800 200 200` (Large number of philosophers)

## 🐛 Debugging

- Use the `-g` flag during compilation for GDB support
- Check for race conditions and deadlocks
- Validate proper mutex usage and thread synchronization

## 📊 Performance Considerations

- Optimized for minimal CPU usage
- Efficient memory management
- Scalable with a large number of philosophers

## 🎓 Learning Outcomes

- Deep understanding of multi-threading concepts
- Practical experience with mutex synchronization
- Problem-solving skills for concurrent programming challenges


## 📝 License

This project is [MIT](https://opensource.org/licenses/MIT) licensed.

## 🌈 Conclusion

The Dining Philosophers project is not just a simulation - it's a journey into the heart of concurrent programming challenges. As you watch your philosophers think, eat, and occasionally starve, remember: in the world of multi-threading, timing is everything!

Happy coding, and may your philosophers dine in harmony! 🍽️🎉

---

<p align="center">
  <i>"To eat is a necessity, but to eat intelligently is an art." - François de La Rochefoucauld</i>
</p>