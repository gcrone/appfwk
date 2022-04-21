namespace dunedaq::appfwk {

template<typename Child>
void
DAQModule::register_command(const std::string& cmd_name, void (Child::*f)(const data_t&), const std::set<std::string>& states)
{
  using namespace std::placeholders;
  
  bool done = m_commands.emplace(cmd_name, std::make_pair(states, std::bind(f, dynamic_cast<Child*>(this), _1))).second;
  if (!done) {
    // Throw here
    throw CommandRegistrationFailed(ERS_HERE, get_name(), cmd_name);
  }
}

} // namespace dunedaq::appfwk
