
from dataclasses import dataclass
from typing import List, Dict, Tuple, Set

@dataclass
class Binary:
    id: int
    services: List[int]
    working_on: int
    last_move: int
    last_work: int

    # se bisogna spostare un servizio da/verso il binary, nessuno deve essere a lavoro
    def can_move(self, day: int) -> bool:
        return max(self.last_move, self.last_work) <= day

    # si può lavorare su un binary solo se non è in corso una migrazione
    def can_work(self, day: int) -> bool:
        return self.last_move <= day


@dataclass
class Service:
    id: int
    name: str
    binary: int
    impl_features: List[str]


@dataclass
class Feature:
    idx: int
    name: str
    difficulty: int
    users: int
    dep_services: List[List]
    day_completed: int
    binaries: Set[int]
    
    # ritorna False appena trova un servizio non implementato, altrimenti True
    def is_finished(self) -> bool:
        for (service, completed) in self.dep_services:
            if not completed:
                return False
        return True



@dataclass
class Engineer:
    id: int
    avail: int
    ops: List[Tuple[str, List[int]]]

    def add_op(self, op_name: str, day: int, duration: int, values: List[int]):
        assert(self.avail <= day)
        if self.avail < day:
            self.ops.append(("wait", [day - self.avail]))

        self.ops.append((op_name, values))
        self.avail = day + duration

score = 0
def run():    
    # lettura dei dati dal file di input
    fname = "data/e.in"

    with open(fname, "r") as f:
            lines = f.readlines()
            
    # parsing della prima riga: dimensione del problema
    L, G, S, B, F, N = [int(k) for k in lines[0].strip().split(" ")]

    # creazione lista degli ingegneri
    engineers: List[Engineer] = []
    for i in range(G):
        engineers.append(Engineer(i, 0, []))
        
    # creazione lista dei binaries
    binaries: List[Binary] = []
    for i in range(B):
        binaries.append(Binary(i, [], 0, 0, 0))

    # creazione service e binary
    services: List[Service] = []
    service_to_id: Dict[str, int] = {}
    for i, (service, binary) in enumerate([line.strip().split() for line in lines[1:S+1]]):
        bid = int(binary)
        b = binaries[bid]
        b.services.append(i)

        s = Service(i, service, bid, [])
        service_to_id[service] = i
        services.append(s)
            
    # creazione features
    features: List[Feature]= []
    for i in range(F):
        features.append(Feature(i, "", 0, 0, [], 0, set()))

    n = 0
    for i, feature in enumerate(lines[1+S:]):
        # leggo solo le righe pari
        if i % 2 ==0:
            feature, num_services, difficulty, users = feature.strip().split(" ")
            s = [[service_to_id[s], False] for s in lines[1+S+1+i].split()]
            f = features[n]
            f.name, f.difficulty, f.users, f.dep_services = feature, int(difficulty), int(users), s
            n += 1

    daily_schedule: List[Tuple[int, int]] = []

    def process_timeline(day: int):
        while len(daily_schedule) > 0 and daily_schedule[0][0] <= day:
            bid: int = daily_schedule[0][1]
            binaries[bid].working_on -=1
            assert(binaries[bid].working_on >= 0)
            daily_schedule.pop(0)

    def implement_feature(e: int, day: int, f: int, b: int) -> int:
        # verifico la correttezza dell'input
        assert(b >= 0 and b < B)
        assert(day >=0 and day < L)
        assert(f >= 0 and f < F)
        assert(e >= 0 and e < G)
        global score
        # controllo eventuali lavori finiti
        process_timeline(day)
        
        feature = features[f]
        was_finished = feature.is_finished()

        # controllo che il binary non sia bloccato
        assert(binaries[b].last_move <= day)

        # calcolo della durata in accordo con la specifica
        duration  = feature.difficulty + len(binaries[b].services) + binaries[b].working_on

        # imposto come implementati tutti i servizi della feature f nel binary b
        for i, (service, done) in enumerate(feature.dep_services):
            if services[service].binary == b and not done:
                feature.dep_services[i][1] = True
                feature.day_completed = max(day + duration, feature.day_completed)
                services[service].impl_features.append(f)
        
        # se la feature è finita e non era terminata incremento il punteggio
        if feature.is_finished() and not was_finished:
            # print("done feature", f)
            score += feature.users * max(0, L-feature.day_completed) 

        # aggiungo l'operazione all'ingegnere, aggiungo alla tabella di scheduling e incremento il numero di persone al lavoro
        engineers[e].add_op("impl", day, duration, [f, b])
        daily_schedule.append((day + duration, b))
        binaries[b].working_on+=1
        
        # print("debug:", "impl",e, day, f, b)
        return duration

    import random

    for day in range(L):
        for engineer in engineers:
            if engineer.avail <= day:
                f: int = random.randint(0, len(features) -1)
                b: int = random.randint(0, len(binaries) -1)

                implement_feature(engineer.id, day, f, b)

    completed_features = [f for f in features if f.is_finished() and f.day_completed < L]
    # print("score = ", score, "completed = ", len(completed_features))
    return score, len(completed_features)

with open("out", "w") as f:
    for i in range(5000):
        score = 0
        score, completed = run()
        f.write(str(score)+" ")
        f.write(str(completed) + "\n")
        if i % 1000 == 0 and i > 0:
            print("done", i)