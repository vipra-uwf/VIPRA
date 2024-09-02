
import util.code_gen as code_gen
import util.modules as modules
import util.args as arg
import util.sim_cache as cache
import util.sim_runner as runner


def build_sim(configpath, installpath):
  code_gen.build_sim(modules.load_config(configpath), installpath)

if __name__ == '__main__':
  args = arg.parse()

  cache = cache.SimCache()
  cache.init(args.cachedir)

  cache.add_sim(args.name, args.file)
  if (args.compile):
    build_sim(cache.config_path(args.name), cache.path(args.name))
  
  if (args.run):
    runner.run(cache.path(args.name))
