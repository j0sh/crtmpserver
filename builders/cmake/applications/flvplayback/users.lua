users=
{
	broadcast="vbrick",
	user1="password1",
	user2="password2",
	user3="password3",
	gigi="spaima",
	a="a",
}

realms=
{
	{
		name="My realm...",
		method="Digest",
		users={
			"user1",
			"user3"
		},
	},
	{
		name="My second realm...",
		method="Basic",
		users={
			"user2",
			"broadcast",
			"user1",
			"gigi"
		},
	}
}

