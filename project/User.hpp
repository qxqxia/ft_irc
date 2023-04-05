using	namespace std;

class	User
{

private:
	string	m_nickname;
	string	m_username;
	string	m_hostname;
	string	m_realname;
	string	m_mode;

	set<string> m_channels;

public:
	User(
		string nick,
		string user,
		string hostname,
		string realname
	);

	~User();

	string	get_nickname() const;
	string	get_username() const;
	string	get_hostname() const;
	string	get_realname() const;

	set<string>	& get_channels();

	string	get_mode() const;

};


#endif



