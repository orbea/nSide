auto Icarus::megaDriveManifest(string location) -> string {
  vector<uint8_t> buffer;
  concatenate(buffer, {location, "program.rom"});
  return megaDriveManifest(buffer, location);
}

auto Icarus::megaDriveManifest(vector<uint8_t>& buffer, string location) -> string {
  if(settings["icarus/UseDatabase"].boolean()) {
    auto digest = Hash::SHA256(buffer).digest();
    for(auto game : Database::MegaDrive.find("game")) {
      if(game["sha256"].text() == digest) return BML::serialize(game);
    }
  }

  if(settings["icarus/UseHeuristics"].boolean()) {
    Heuristics::MegaDrive game{buffer, location};
    if(auto manifest = game.manifest()) return manifest;
  }

  return {};
}

auto Icarus::megaDriveImport(vector<uint8_t>& buffer, string location) -> string {
  auto name = Location::prefix(location);
  auto source = Location::path(location);
  string target{settings["Library/Location"].text(), "Mega Drive/", name, ".md/"};

  auto manifest = megaDriveManifest(buffer, location);
  if(!manifest) return failure("failed to parse ROM image");

  if(!create(target)) return failure("library path unwritable");
  if(exists({source, name, ".sav"}) && !exists({target, "save.ram"})) {
    copy({source, name, ".sav"}, {target, "save.ram"});
  }

  if(settings["icarus/CreateManifests"].boolean()) write({target, "manifest.bml"}, manifest);
  write({target, "program.rom"}, buffer);
  return success(target);
}
