#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/crypto.hpp>

using namespace eosio;

using std::string;

CONTRACT prove : public contract {
   public:
      using contract::contract;

		  struct [[eosio::table]] worbliclaim
		  {
		    name              worbli_account;
		    asset            	edna_amount;
		    public_key   			pubkey;
		    uint64_t          primary_key() const { return worbli_account.value; }

		    EOSLIB_SERIALIZE( worbliclaim, (worbli_account)(edna_amount)(pubkey) )

		  };

      ACTION loadclaim(name worbli_account, asset edna_amount, public_key pubkey);
      ACTION worbclaim(name ux_account, name worbli_account, signature sig);
      ACTION endclaim();
      ACTION clear();

      using loadclaim_action = action_wrapper<"loadclaim"_n, &prove::loadclaim>;
      using worbclaim_action = action_wrapper<"worbclaim"_n, &prove::worbclaim>;
      using endclaim_action = action_wrapper<"endclaim"_n, &prove::endclaim>;
      using clear_action = action_wrapper<"clear"_n, &prove::clear>;

    private:
    	typedef multi_index<"worbliclaim"_n, worbliclaim> worbliclaims_table;

};