#pragma once

class Avm {
	public:
		Avm();
		Avm(Avm const &src);
		virtual ~Avm();

		Avm &operator=(Avm const &rhs);
	protected:
	private:
};
