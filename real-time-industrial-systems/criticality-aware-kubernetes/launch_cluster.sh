#!/bin/bash 

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

containerd -c config/containerd_config.toml &

# start etcd 
./etcd/etcd-v3.5.13-linux-amd64/etcd &

kube-controller-manager --kubeconfig=config/kubelet.kubeconfig --kube-api-qps -1 &

# start api-server
kube-apiserver --etcd-servers=http://127.0.0.1:2379 \
   --service-cluster-ip-range=10.0.0.0/16 \
   --service-account-issuer=https://kubernetes.default.svc.cluster.local \
   --service-account-signing-key-file=config/certs/service-account-key.pem \
   --service-account-key-file=config/certs/service-account-pub.pem \
   --token-auth-file=config/token_auth_file &

# start scheduler
kube-scheduler --kubeconfig config/kubelet.kubeconfig &

# --kubelet-cgroups=/kubelet \
# start kubelet

# --reserved-cpus=0,1 \
sleep 5
kubelet --config config/config.yaml --logging-format=json \
  --container-runtime-endpoint=unix:///run/containerd/containerd.sock \
  --kubeconfig config/kubelet.kubeconfig &> a.json &

sleep 5
# chrt -f -p -a 99 $(pidof kubelet)
# chrt -f -p -a 99 $(pidof containerd)
# taskset -ap --cpu-list 0-1  $(pidof kubelet)
# taskset -ap --cpu-list 2-3 $(pidof containerd)
# chrt -f -p -a 40 $(pidof kube-scheduler)
# chrt -f -p -a 41 $(pidof kube-apiserver)
# chrt -f -p -a 52 $(pidof kube-controller-manager)
# chrt -f -p -a 39 $(pidof etcd)

wait
