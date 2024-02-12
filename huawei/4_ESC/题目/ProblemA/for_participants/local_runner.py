import sys

from dataclasses import dataclass
from pathlib import Path
from subprocess import Popen, PIPE
    

def wrong_answer(participant, reason):
    participant.stdin.write('10000000 END 0\n')
    try:
        participant.stdin.flush()
    except:
        pass
    print(f'Wrong Answer: {reason}', file=sys.stderr)
    sys.exit(1)


@dataclass
class Flavor:
    cpu: int
    mem: int
    cost: float


@dataclass
class Pod:
    cpu: int
    mem: int
    node: int
    idx: int


@dataclass
class Node:
    cpu: int
    mem: int
    cost: float
    start: int
    idx: int

    def allocate(self, pod):
        if self.cpu < pod.cpu:
            return f'Trying to place pod {pod.idx} on node {self.idx}, but it has not enough CPU: {self.cpu} vs. {pod.cpu} required by pod.'
        if self.mem < pod.mem:
            return f'Trying to place pod {pod.idx} on node {self.idx}, but it has not enough memory: {self.mem} vs. {pod.mem} required by pod.'
        self.cpu -= pod.cpu
        self.mem -= pod.mem
        return None

    def deallocate(self, pod):
        self.cpu += pod.cpu
        self.mem += pod.mem


def main(in_file, baseline, participant_run_cmd):
    participant = Popen(participant_run_cmd, stdin=PIPE, stdout=PIPE, text=True, encoding='utf-8')
    input_ = in_file.read_text().splitlines()
    n_flavors = int(input_[0])
    participant.stdin.write(input_[0] + '\n')
    flavors = []
    for i in range(1, n_flavors + 1):
        participant.stdin.write(input_[i] + '\n')
        tokens = input_[i].split()
        flavors.append(Flavor(cpu=int(tokens[0]), mem=int(tokens[1]), cost=float(tokens[2])))
    participant.stdin.flush()
    input_ptr = n_flavors + 1
    pods = []
    nodes = []
    node_deleted = []
    node_pods = []
    total_cost = 0.0
    while True:
        participant.stdin.write(input_[input_ptr] + '\n')
        participant.stdin.flush()
        timestamp, kind, size = input_[input_ptr].split()
        input_ptr += 1
        timestamp = int(timestamp)
        size = int(size)
        if kind == 'CREATE':
            curr_pods = []
            for i in range(size):
                participant.stdin.write(input_[input_ptr] + '\n')
                idx, cpu, mem = list(map(int, input_[input_ptr].split()))
                input_ptr += 1
                curr_pods.append(Pod(cpu=cpu, mem=mem, node=-1, idx=idx))
            participant.stdin.flush()
            n_nodes, *new_nodes = list(map(int, participant.stdout.readline().split()))
            if n_nodes != len(new_nodes):
                wrong_answer(participant, f'request at timestamp {timestamp}: expected {n_nodes} nodes, found {len(new_nodes)}')
            if n_nodes > 100:
                wrong_answer(participant, f'request at timestamp {timestamp}: too many nodes: got {n_nodes}, but <= 100 allowed.')
            for node in new_nodes:
                if node < 1 or node > n_flavors:
                    wrong_answer(participant, f'request at timestamp {timestamp}: invalid node flavor {node}.')
                idx = len(nodes) + 1
                nodes.append(Node(cpu=flavors[node - 1].cpu, mem=flavors[node - 1].mem, cost=flavors[node - 1].cost, start=timestamp, idx=idx))
                node_deleted.append(None)
                node_pods.append(set())
            placement = list(map(int, participant.stdout.readline().split()))
            if len(placement) != size:
                wrong_answer(participant, f'request at timestamp {timestamp}: got {len(placement)} node indexes for pod placement, but request has {size} pods.')
            for i in range(size):
                idx = placement[i]
                if idx < 1 or idx > len(nodes):
                    wrong_answer(participant, f'request at timestamp {timestamp}: non-existing node id {idx}.')
                if node_deleted[idx - 1] is not None:
                    wrong_answer(participant, f'request at timestamp {timestamp}: node id {idx} belongs to a deleted node.')
                err = nodes[idx - 1].allocate(curr_pods[i])
                if err:
                    wrong_answer(participant, f'request at timestamp {timestamp} ' + err)
                node_pods[idx - 1].add(curr_pods[i].idx)
                curr_pods[i].node = idx - 1
            for i in range(n_nodes):
                if len(node_pods[-n_nodes+i]) == 0:
                    node_deleted[-n_nodes+i] = timestamp
            pods += curr_pods
        elif kind == 'DELETE':
            participant.stdin.write(input_[input_ptr] + '\n')
            participant.stdin.flush()
            ids = list(map(int, input_[input_ptr].split()))
            input_ptr += 1
            for idx in ids:
                node = pods[idx - 1].node
                node_pods[node].remove(idx)
                nodes[node].deallocate(pods[idx - 1])
                if len(node_pods[node]) == 0:
                    node_deleted[node] = timestamp
                    total_cost += nodes[node].cost * (timestamp - nodes[node].start)
        elif kind == 'END':
            break
        else:
            raise ValueError(f"Unknown request type: {kind}")
    if baseline is None:
        print('OK: cluster cost = {:.4f}'.format(total_cost))
    else:
        print('OK: score = {:.8f}'.format(((baseline - round(total_cost, 4)) / baseline + 1.0) * 1000))


def read_baseline_score(filename):
    return float(filename.read_text().rstrip())


if __name__ == '__main__':
    in_file = sys.argv[1]
    split = sys.argv.index('--')
    assert split == 2 or split == 3
    baseline = read_baseline_score(Path(sys.argv[2])) if split == 3 else None
    main(Path(in_file), baseline, sys.argv[split+1:])
