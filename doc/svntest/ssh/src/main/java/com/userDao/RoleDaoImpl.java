package com.userDao;



import com.userEntity.Role;
import com.userEntity.Rolepre;
import com.userEntity.Roles;
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
 * Created by churui on 2017/7/12.
 */
@Repository("roleDao")
public class RoleDaoImpl extends HibernateDaoSupport implements RoleDao{
    @Resource(name = "sessionFactory")
    public void setSessionFactoryOverride(SessionFactory sessionFactory)
    {
        super.setSessionFactory(sessionFactory);

    }
    @Override

    public void save(Role role) {
              this.getHibernateTemplate().save(role);
    }

    @Override

    public Role findRole(String rolename) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
        String hql = "from Role where rolename=:name";
        Query query = session.createQuery(hql);
        query.setString("name",rolename);
        Role role = (Role)query.uniqueResult();
        session.close();
        return role;
    }

    @Override
    public List<Role> findbynodeid(Integer nodeid) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
        String hql = "from Role where nodetbl.nodeid=:nodeid";
        Query query = session.createQuery(hql);
        query.setInteger("nodeid",nodeid);
        List<Role> list = query.list();
        session.close();
        return list;
    }

    @Override
    public void deleterole(Integer roleid) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
        String hql = "delete from Role as r where r.id=?";
        Query query = session.createQuery(hql);
        query.setInteger(0,roleid);
        query.executeUpdate();
        session.close();
    }

    @Override
    public void update(Role role) {
        this.getHibernateTemplate().update(role);
    }

    @Override
    public List<Role> findall() {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
        String hql = "from Role ";
        Query query = session.createQuery(hql);
        List<Role> list = query.list();
        session.close();
        return list;
    }

    @Override
    public Role findbyroleid(Integer roleid) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
        String hql = "from Role where roleid=:id";
        Query query = session.createQuery(hql);
        query.setInteger("id",roleid);
        Role role = (Role)query.uniqueResult();
        session.close();
        return role;
    }

    @Override
    public List<Roles> findrolesbynode(Integer nodeid) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
        String hql = "from Roles where nodetbl.nodeid=:nodeid";
        Query query = session.createQuery(hql);
        query.setInteger("nodeid",nodeid);
        List<Roles> list = query.list();
        session.close();
        return list;
    }

    @Override
    public void saveroles(Roles roles) {
        this.getHibernateTemplate().save(roles);
    }

    @Override
    public Roles findrolesbyname(String rname) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
        String hql = "from Roles where rname=:name";
        Query query = session.createQuery(hql);
        query.setString("name",rname);
        Roles roles = (Roles)query.uniqueResult();
        session.close();
        return roles;
    }

    @Override
    public Roles findrolesbyrid(Integer rid) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
        String hql = "from Roles where rid=:id";
        Query query = session.createQuery(hql);
        query.setInteger("id",rid);
        Roles roles= (Roles)query.uniqueResult();
        session.close();
        return roles;
    }

    @Override
    public void updateroles(Roles roles) {
        this.getHibernateTemplate().update(roles);
    }

    @Override
    public void deleteroles(Integer rid) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
        String hql = "delete from Roles as r where r.rid=?";
        Query query = session.createQuery(hql);
        query.setInteger(0,rid);
        query.executeUpdate();
        session.close();
    }

    @Override
    public List<Role> findrolebyrid(Integer rid) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
        String hql = "from Role where roles.rid=:rid";
        Query query = session.createQuery(hql);
        query.setInteger("rid",rid);
        List<Role> list = query.list();
        session.close();
        return list;
    }

    @Override
    public Roles findrolesbynodeidandname(Integer nodeid, String rname) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
        String hql = "from Roles where nodetbl.nodeid=:nodeid and rname=:rname";
        Query query = session.createQuery(hql);
        query.setInteger("nodeid",nodeid);
        query.setString("rname",rname);
        Roles roles = (Roles) query.uniqueResult();
        session.close();
        return roles;
    }

    @Override
    public void addrolepre(Rolepre rolepre) {
        this.getHibernateTemplate().save(rolepre);
    }

    @Override
    public Rolepre findroleprebyrid(Integer rid) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
        String hql = "from Rolepre where roles.rid=:rid";
        Query query = session.createQuery(hql);
        query.setInteger("rid",rid);
        Rolepre rolepre = (Rolepre)query.uniqueResult();
        session.close();
        return rolepre;
    }

    @Override
    public void updaterolepre(Rolepre rolepre) {
        this.getHibernateTemplate().update(rolepre);
    }

    @Override
    public void deleterolepre(Integer rid) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
        String hql = "delete from Rolepre as r where r.roles.rid=?";
        Query query = session.createQuery(hql);
        query.setInteger(0,rid);
        query.executeUpdate();
        session.close();
    }

    @Override
    public void sysdate() {

    }
}
