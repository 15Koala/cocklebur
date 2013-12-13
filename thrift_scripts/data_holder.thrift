/*
 * Log 记录
 * 结合快照文件做日志回滚，恢复内存状态
 * oper: type | [data|path] 
 *	 c nodeCreate
 *	 d nodeDelete
 *	 s setData
 */
struct LogEntry {
    1: i64 xid,
    2: i64 ts,
    3: byte oper, 
    4: string content,
}

/*
 * 目录树中的一个节点
 */
struct Node {
    1: set<string> children,
    2: string parent,
    3: i64 ctime,
    4: i64 mtime,
    5: string data,
    6: i64 data_version,
    7: string path,
}

/*
 * 目录树，内存快照的最终依据，保存了该节点所维护数据的内存状态。
 */
struct DataTree {
    1: map<string, Node> NodeMap,
    2: i64 xid,
    3: map<string, Node> ephemerals,
}
/*
 * create 阻塞和非阻塞创建目录，阻塞创建往往用于抢占加分布式锁的场景
 * 状态码：0 成功，1 父目录不存在，2 非阻塞下目录已存在
 * delete 删除指定path
 * 状态码：0 成功，1 目录不存在
 * exist 判断目录是否存在
 * getData/ setData
 * 状态码：0 成功，1 目录不存在
 *
 * nodeCreate, nodeDelete => asyn
 * nodeCreate  => syn
 * other => syn
 */
service DataServ{
    i32 nodeCreate(1: string path, 2: bool isBlocked),
    i32 nodeDelete(1: string path),
    bool nodeExist(1: string path),
    string getData(1: string path),
    void setData(1: string path, 2: string data),
    list<Node> getChildren(1: string path),
    DataTree getDataTree(),
    list<LogEntry> getLogEntryList(1: i64 xid)
}
