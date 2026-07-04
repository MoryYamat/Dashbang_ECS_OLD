// true false

#pragma once


namespace Game::Common::Logic::FSM::Condition
{
	// í‚Étrue‚ğ•Ô‚·ğŒ
	template<typename Context>
	struct TrueCondition : ICondition<Context>
	{
		bool evaluate(const Context&) const override
		{
			return true;
		}
	};

	// í‚Éfalse‚ğ•Ô‚·ğŒ
	template<typename Context>
	struct FalseCondition : ICondition<Context>
	{
		bool evaluate(const Context&) const override
		{
			return false;
		}
	};
}