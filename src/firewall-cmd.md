# firewall-cmd

______________________________________________________________________

**Date:** 2026-08-06
**Tags:**[Firewall.md](tags/Firewall.md)
**URL:**[secure-linux-network-firewall-cmd](https://www.redhat.com/en/blog/secure-linux-network-firewall-cmd)

______________________________________________________________________

enable firewall-cmd

```bash
sudo systemctl enable --now firewalld
```

get zones 
In firewalld, a zone is a named set of firewall rules representing a trust level. You attach each network interface (or source IP range "192.168.0.0/24") to one zone, and that zone decides what incoming traffic is allowed.

```bash
sudo firewall-cmd --get-zones
```

see whats unblocked
```bash
sudo firewall-cmd --zone work --list-all
work
target: default
icmp-block-inversion: no
interfaces: ens3
sources: 
services: cockpit dhcpv6-client ssh
ports: 
protocols: 
masquerade: no
forward-ports: 
source-ports: 
icmp-blocks: 
rich rules:
```
create a zone
```bash
sudo firewall-cmd --new-zone corp --permanent # add permanent to last restarts of the computer or the firewall
sucess
sudo firewall-cmd --reload
```

`--permanent` writes the rule to disk but doesn't apply it; `--reload` applies it without
dropping existing connections. without `--zone`, a command acts on the default zone.

assign an interface to a zone
```bash
sudo firewall-cmd --change-interface ens3 --zone corp --permanent
```

set the default zone
```bash
sudo firewall-cmd --set-default-zone corp
```

see which zones are in use and on which interfaces
```bash
sudo firewall-cmd --get-active-zones
```

list the predefined services (names that map to known ports)
```bash
sudo firewall-cmd --get-services
```

allow / block a service
```bash
sudo firewall-cmd --zone corp --add-service ssh --permanent
sudo firewall-cmd --remove-service http --permanent
```

open / close a specific port
```bash
sudo firewall-cmd --add-port 1622/tcp --permanent
sudo firewall-cmd --remove-port 1622/tcp --permanent
```

