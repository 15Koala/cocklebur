/*
 * hostStat holds host's stat data.
 *
 * xid: data version of this host, the newer ,the larger.
 * knownHosts: ensure the xid is the largest int this set.
 * heardFrom: has exchanged vote with them.
 * myHostName: my host name.
 * recHost: current host's name who has the largest xid;
 */
struct HostStat {
    1: i64 xid,
    2: set<string> known_hosts,
    3: set<string> heard_from,
    4: set<string> alive_hosts,
    5: string my_host_name,
    6: string rec_host,
    7: i64 logical_clock,
}
/*
 * a part of hostStat to send others. Nodes communicate with each other with it.
 */
struct Voter {
    1: string rec_host,
    2: string my_host_name,
    3: i64 xid,
    4: set<string> known_hosts,
    5: i64 logical_clock,
    6: i32 cur_node_mode,
}

struct HBMSG {
    1: string my_host_name, 
    2: i32 cur_node_mode,
    3: i64 xid,
}

/*
 * leader election service. It has 3 methods
 */
service ElectionServ{
    Voter exchange(1: Voter v),
    bool isLargest(1: Voter v),
    oneway void sendMessage(1: string message)
}
/*
 * cocklebur common service.
 */
service CockleburServ{
    HostStat getHostStat(),
    string ping(1: string message),
    HBMSG ping2(1: HBMSG message)
}

