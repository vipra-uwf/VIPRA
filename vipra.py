
import tools.sim_builder.code_gen as code_gen
import tools.sim_builder.modules as modules
import tools.sim_builder.args as arg
import tools.sim_builder.sim_cache as cache
import tools.sim_builder.sim_runner as runner


def build_sim(configpath, installpath):
  code_gen.build_sim(modules.load_config(configpath), installpath)

if __name__ == '__main__':
  args = arg.parse()

  cache = cache.SimCache()
  cache.init('./simcache')

  if (not args.compile and not args.run):
    # new simulation
    cache.configure_sim(args.name)

  if (args.compile):
    if cache.configured(args.name):
      build_sim(cache.config_path(args.name), cache.path(args.name))
    else:
      print(f'Simulation {args.name} was not configured\nRun: `python3 vipra.py -n {args.name}` to configure')
  
  if (args.run):
    if cache.configured(args.name):
      runner.run(cache.path(args.name), args.name, cache)
    else:
      print(f'Simulation {args.name} was not configured\nRun: `python3 vipra.py -n {args.name}` to configure')
