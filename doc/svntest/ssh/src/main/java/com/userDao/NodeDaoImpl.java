package com.userDao;

import com.userEntity.Node;
import org.hibernate.*;
import org.springframework.orm.hibernate3.HibernateCallback;
import org.springframework.stereotype.Repository;
import org.springframework.orm.hibernate3.support.HibernateDaoSupport;
import org.springframework.transaction.annotation.Transactional;

import javax.annotation.Resource;
import java.io.Serializable;
import java.sql.SQLException;
import java.util.List;

/**
 * Created by churui on 2017/7/26.
 */
@Repository("nodeDao")
public class NodeDaoImpl extends HibernateDaoSupport implements NodeDao{
    @Resource(name = "sessionFactory")
    public void setSessionFactoryOverride(SessionFactory sessionFactory)
    {
        super.setSessionFactory(sessionFactory);

    }
    @Override
    public void addnode(Node node) {
//        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
//        Transaction transaction = session.beginTransaction();
//        transaction.begin();
//        Serializable id =session.save(node);
        Serializable id = this.getHibernateTemplate().save(node);
//        Node newNode = (Node) session.get(Node.class, id);

        //       transaction.commit();
        /*事务织入本身没有问题,错误处在这个session关闭的代码,session关闭后也就不能提交事务了
        * 设置了SessionFactory以后,不需再显性的获取session,会话工厂会自行获取和关闭session
        * 直接通过HibernateTemplate自带的方法操作数据库即可
        * */
//        session.close();
    }

    @Override
    public List<Node> findall() {
       Session session = this.getHibernateTemplate().getSessionFactory().openSession();
        String hql = "from Node";
        Query query = session.createQuery(hql);
        List<Node> list = query.list();
        session.close();
        return list;
    }

    @Override
    public void deletenode(Integer nodeid) {
   //     Session session = this.getHibernateTemplate().getSessionFactory().openSession();
//        Transaction transaction = session.beginTransaction();
//        transaction.begin();
        Node node = (Node) this.getHibernateTemplate().load(Node.class,nodeid);
        this.getHibernateTemplate().delete(node);
 //       transaction.commit();
  //      session.close();
    }

    @Override
    public Node findnodebyname(String nodename) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
        String hql = "from Node where nodename=:name";
        Query query = session.createQuery(hql);
        query.setString("name",nodename);
        Node node = (Node)query.uniqueResult();
        session.close();
        return node;
    }

    @Override
    public List<Node> findnodeidbyparentid(Integer parentid) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
        String hql = "from Node where parentid=:id";
        Query query = session.createQuery(hql);
        query.setInteger("id",parentid);
        List<Node> list = query.list();
        session.close();
        return list;
    }

    @Override
    public Node findnodebyid(Integer nodeid) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
        String hql = "from Node where nodeid=:id";
        Query query = session.createQuery(hql);
        query.setInteger("id",nodeid);
        Node node = (Node)query.uniqueResult();
        session.close();
        return node;
    }

    @Override
    public void updatenode(Node node) {
 //       Session session = this.getHibernateTemplate().getSessionFactory().openSession();
//        Transaction transaction = session.beginTransaction();
//        transaction.begin();
        Node nodes = (Node)this.getHibernateTemplate().get(Node.class,node.getNodeid());
        nodes.setNodename(node.getNodename());
//        transaction.commit();
  //      session.close();
    }
    @Override
    public List<Node> findNextLevelChildren(Node nodetbl) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
        String hql = "FROM Node unit WHERE unit.parentid =:nodetblId";
        Query query = session.createQuery(hql);
        query.setInteger("nodetblId",nodetbl.getNodeid());
        List<Node> nodeList = query.list();
        session.close();
        return nodeList;
    }
}
