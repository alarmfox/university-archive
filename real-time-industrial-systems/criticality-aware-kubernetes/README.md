# Criticality-aware-kubernetes
Progetto per il corso di real-time industrial systems

## Note

### Operator pattern
Estensioni che permettono di modificare il comportamento di un pod. Si potrebbe 
usare il MetaController che gira le richieste (con un webhook) ad una funzione custom
definita da noi (Lambda controller).
* Decorator pattern: potrebbe essere usato per aggiungere un comportamento real-time ai 
pod.

1. Capire: custom resource defintion. Cosa fa la kubelet per lanciare container. Output: grafico flow, syncLoop
2. Voglio dedicare un set di risorse fisse per lanciare un container: kubelet e daemon in uno stesso cgroup?
3. Definire una risorsa pod-rt. Come usare differenti runtime? Ad esempio non voglio usare runc, ma voglio usare runphi.


### Note codice Kubernetes

* Definizione Cgroup da usare per la kubelet: ```cmd/kubelet/app/server.go:561``` func run(...);
* SystemCgroup: ```cmd/kubelet/app/server.go:724```
