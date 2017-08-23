package com.userDao;

import com.userEntity.User;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.springframework.orm.hibernate3.support.HibernateDaoSupport;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import javax.annotation.Resource;
import java.io.Serializable;
import java.util.List;

/**
 * Created by churui on 2017/7/11.
 */
@Repository("userDao")
public class UserDaoImpl extends HibernateDaoSupport implements UserDao{
    @Resource(name = "sessionFactory")
    public void setSessionFactoryOverride(SessionFactory sessionFactory)
    {
        super.setSessionFactory(sessionFactory);

    }
    @Override

    public String saveuser(User user) {
 ///       Session session = this.getHibernateTemplate().getSessionFactory().openSession();
//        Transaction transaction = session.beginTransaction();
//       transaction.begin();
        Serializable id =this.getHibernateTemplate().save(user);
//        transaction.commit();
//        session.close();
        if (null==id){
            return "fail";
        }else{
            return "success";
        }
    }
    @Override
    public List<User> findall(Integer nodeid) {
            Session session = this.getHibernateTemplate().getSessionFactory().openSession();
            String hql = "from User where nodetbl.nodeid=:nodeid";
            Query query = session.createQuery(hql);
            query.setInteger("nodeid",nodeid);
            List<User> list = query.list();
            session.close();
            return list;
    }
    @Override

    public void deleteuser(Integer id) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
//        Transaction transaction = session.beginTransaction();
//        transaction.begin();
        String hql = "delete from User as u where u.id=?";
        Query query = session.createQuery(hql);
        query.setInteger(0,id);
        query.executeUpdate();
       // transaction.commit();
       session.close();
    }

    @Override

    public void updateuser(User user) {
  //      Session session = this.getHibernateTemplate().getSessionFactory().openSession();
//        Transaction transaction = session.beginTransaction();
//        transaction.begin();
        this.getHibernateTemplate().update(user);
 //       transaction.commit();
 //       session.close();
    }

    @Override
    public List<User> findall() {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
//        Transaction transaction = session.beginTransaction();
//        transaction.begin();
        String hql = "from User ";
        Query query = session.createQuery(hql);
        List<User> list = query.list();
//        transaction.commit();
        session.close();
        return list;
    }

    @Override
    public User finduserbyname(String name) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
        String hql = "from User where name=:name";
        Query query = session.createQuery(hql);
        query.setString("name",name);
        User user = (User)query.uniqueResult();
        session.close();
        return user;
    }
}
