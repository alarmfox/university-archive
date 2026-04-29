# Criticality-aware Kubernetes

This repository contains the research project **"Criticality aware Kubernetes"**, developed for the **Real-Time Industrial Systems** course (A.Y. 2023/2024) at the **University of Naples Federico II**.

The project investigates the feasibility of using Kubernetes for **soft real-time applications**, focusing specifically on the **Kubelet**'s internal mechanisms and the factors influencing **pod startup latency** in Linux environments.

## 📖 Project Overview

Kubernetes is traditionally designed for high-availability cloud workloads, not real-time constraints. This research explores:
- **Kubelet Synchronization Loop:** Analysis of the `syncLoop` and how it handles pod updates.
- **Container Runtime Interaction:** Deep dive into how Kubelet interacts with `containerd` and `runc` via the Container Runtime Interface (CRI).
- **Latency Modeling:** Development of a theoretical model for the upper bound of pod startup latency ($L_{wn}$).
- **Performance Optimization:** Evaluating the impact of the Linux OS scheduler (`SCHED_FIFO`), CPU isolation (`isolcpus`), and `cgroups` on startup determinism.

## 🏗️ Architecture & Theoretical Model

The project models the total creation latency $L_{pod}$ as:
$$L_{pod}(S) = L_{cp}(S_{cp}) + L_{wn}(S_{wn}) + \delta$$

Where the worker node latency ($L_{wn}$) is further decomposed into:
- $t_w$: Queuing time in the Kubelet.
- $t_i$: Image preparation/download time.
- $t_{runc}$: Execution time of system calls for container creation (approx. 11 syscalls like `clone`, `unshare`, `pivot_root`).

## 🚀 Getting Started

### Prerequisites
- Linux OS (Tested on `Linux taurino 6.9.2-artix1-1` with `PREEMPT_DYNAMIC`)
- `containerd`, `etcd`, `kube-apiserver`, `kube-controller-manager`, `kube-scheduler`, `kubelet`, and `kubectl` binaries.
- Python 3.x and Jupyter Notebook.

### 1. Launch the Cluster
The `launch_cluster.sh` script initializes a minimal standalone Kubernetes environment:
```bash
sudo ./launch_cluster.sh
```
This script starts `containerd`, `etcd`, the Control Plane components, and the `kubelet` with JSON logging enabled.

### 2. Run Experiments
The `main.py` script automates the deployment of workloads (e.g., `nginx`) with varying pod counts and repetitions:
```bash
python main.py --image nginx:latest --repetitions 20
```

### 3. Data Analysis
Experimental results are stored in the `logs/` directory. Use the `analyze.ipynb` notebook to perform statistical analysis, including:
- **Violin and Box Plots** for distribution visualization.
- **Extreme Value Theory (EVT):** Fitting data to the Generalized Extreme Value (GEV) distribution to estimate probabilistic Worst-Case Execution Time (WCET).

## 📊 Key Findings

- **Scheduler Impact:** The Linux OS scheduler is the most significant factor influencing latency. Switching the Kubelet and Container Runtime to `SCHED_FIFO` significantly reduces both average latency and variance.
- **CPU Isolation:** Using `isolcpus` to dedicate cores to the Kubelet and containerd provides the most deterministic results by avoiding OS housekeeping interference.
- **cgroups:** Separating `containerd` and `kubelet` into different `cgroups` did not show a substantial advantage compared to running them in the same group under real-time scheduling.

## 📁 Repository Structure

- `config/`: Kubernetes and Containerd configuration files.
- `doc/`: LaTeX source for the final report and presentation.
- `logs/`: Raw JSON logs from various experimental runs (e.g., `nocgroup`, `fifo`, `isolcpus`).
- `analyze.ipynb`: Jupyter notebook for data processing and GEV fitting.
- `main.py`: Test orchestration script.
- `launch_cluster.sh`: Environment setup script.

## 👥 Authors
- **Giuseppe Capasso**
