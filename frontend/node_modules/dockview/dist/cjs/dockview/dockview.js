"use strict";
var __assign = (this && this.__assign) || function () {
    __assign = Object.assign || function(t) {
        for (var s, i = 1, n = arguments.length; i < n; i++) {
            s = arguments[i];
            for (var p in s) if (Object.prototype.hasOwnProperty.call(s, p))
                t[p] = s[p];
        }
        return t;
    };
    return __assign.apply(this, arguments);
};
var __read = (this && this.__read) || function (o, n) {
    var m = typeof Symbol === "function" && o[Symbol.iterator];
    if (!m) return o;
    var i = m.call(o), r, ar = [], e;
    try {
        while ((n === void 0 || n-- > 0) && !(r = i.next()).done) ar.push(r.value);
    }
    catch (error) { e = { error: error }; }
    finally {
        try {
            if (r && !r.done && (m = i["return"])) m.call(i);
        }
        finally { if (e) throw e.error; }
    }
    return ar;
};
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.DockviewReact = void 0;
var react_1 = __importDefault(require("react"));
var dockview_core_1 = require("dockview-core");
var reactContentPart_1 = require("./reactContentPart");
var reactHeaderPart_1 = require("./reactHeaderPart");
var react_2 = require("../react");
var reactWatermarkPart_1 = require("./reactWatermarkPart");
var headerActionsRenderer_1 = require("./headerActionsRenderer");
function createGroupControlElement(component, store) {
    return component
        ? function (groupPanel) {
            return new headerActionsRenderer_1.ReactHeaderActionsRendererPart(component, store, groupPanel);
        }
        : undefined;
}
var DEFAULT_REACT_TAB = 'props.defaultTabComponent';
function extractCoreOptions(props) {
    var coreOptions = dockview_core_1.PROPERTY_KEYS_DOCKVIEW.reduce(function (obj, key) {
        if (key in props) {
            obj[key] = props[key];
        }
        return obj;
    }, {});
    return coreOptions;
}
exports.DockviewReact = react_1.default.forwardRef(function (props, ref) {
    var domRef = react_1.default.useRef(null);
    var dockviewRef = react_1.default.useRef();
    var _a = __read((0, react_2.usePortalsLifecycle)(), 2), portals = _a[0], addPortal = _a[1];
    react_1.default.useImperativeHandle(ref, function () { return domRef.current; }, []);
    var prevProps = react_1.default.useRef({});
    react_1.default.useEffect(function () {
        var changes = {};
        dockview_core_1.PROPERTY_KEYS_DOCKVIEW.forEach(function (propKey) {
            var key = propKey;
            var propValue = props[key];
            if (key in props && propValue !== prevProps.current[key]) {
                changes[key] = propValue;
            }
        });
        if (dockviewRef.current) {
            dockviewRef.current.updateOptions(changes);
        }
        else {
            // not yet fully initialized
        }
        prevProps.current = props;
    }, dockview_core_1.PROPERTY_KEYS_DOCKVIEW.map(function (key) { return props[key]; }));
    react_1.default.useEffect(function () {
        var _a;
        if (!domRef.current) {
            return;
        }
        var frameworkTabComponents = (_a = props.tabComponents) !== null && _a !== void 0 ? _a : {};
        if (props.defaultTabComponent) {
            frameworkTabComponents[DEFAULT_REACT_TAB] =
                props.defaultTabComponent;
        }
        var frameworkOptions = {
            createLeftHeaderActionComponent: createGroupControlElement(props.leftHeaderActionsComponent, { addPortal: addPortal }),
            createRightHeaderActionComponent: createGroupControlElement(props.rightHeaderActionsComponent, { addPortal: addPortal }),
            createPrefixHeaderActionComponent: createGroupControlElement(props.prefixHeaderActionsComponent, { addPortal: addPortal }),
            createComponent: function (options) {
                return new reactContentPart_1.ReactPanelContentPart(options.id, props.components[options.name], {
                    addPortal: addPortal,
                });
            },
            createTabComponent: function (options) {
                return new reactHeaderPart_1.ReactPanelHeaderPart(options.id, frameworkTabComponents[options.name], {
                    addPortal: addPortal,
                });
            },
            createWatermarkComponent: props.watermarkComponent
                ? function () {
                    return new reactWatermarkPart_1.ReactWatermarkPart('watermark', props.watermarkComponent, {
                        addPortal: addPortal,
                    });
                }
                : undefined,
            defaultTabComponent: props.defaultTabComponent
                ? DEFAULT_REACT_TAB
                : undefined,
        };
        var api = (0, dockview_core_1.createDockview)(domRef.current, __assign(__assign({}, extractCoreOptions(props)), frameworkOptions));
        var _b = domRef.current, clientWidth = _b.clientWidth, clientHeight = _b.clientHeight;
        api.layout(clientWidth, clientHeight);
        if (props.onReady) {
            props.onReady({ api: api });
        }
        dockviewRef.current = api;
        return function () {
            dockviewRef.current = undefined;
            api.dispose();
        };
    }, []);
    react_1.default.useEffect(function () {
        if (!dockviewRef.current) {
            return function () {
                // noop
            };
        }
        var disposable = dockviewRef.current.onDidDrop(function (event) {
            if (props.onDidDrop) {
                props.onDidDrop(event);
            }
        });
        return function () {
            disposable.dispose();
        };
    }, [props.onDidDrop]);
    react_1.default.useEffect(function () {
        if (!dockviewRef.current) {
            return function () {
                // noop
            };
        }
        var disposable = dockviewRef.current.onWillDrop(function (event) {
            if (props.onWillDrop) {
                props.onWillDrop(event);
            }
        });
        return function () {
            disposable.dispose();
        };
    }, [props.onWillDrop]);
    react_1.default.useEffect(function () {
        if (!dockviewRef.current) {
            return;
        }
        dockviewRef.current.updateOptions({
            createComponent: function (options) {
                return new reactContentPart_1.ReactPanelContentPart(options.id, props.components[options.name], {
                    addPortal: addPortal,
                });
            },
        });
    }, [props.components]);
    react_1.default.useEffect(function () {
        var _a;
        if (!dockviewRef.current) {
            return;
        }
        var frameworkTabComponents = (_a = props.tabComponents) !== null && _a !== void 0 ? _a : {};
        if (props.defaultTabComponent) {
            frameworkTabComponents[DEFAULT_REACT_TAB] =
                props.defaultTabComponent;
        }
        dockviewRef.current.updateOptions({
            defaultTabComponent: props.defaultTabComponent
                ? DEFAULT_REACT_TAB
                : undefined,
            createTabComponent: function (options) {
                return new reactHeaderPart_1.ReactPanelHeaderPart(options.id, frameworkTabComponents[options.name], {
                    addPortal: addPortal,
                });
            },
        });
    }, [props.tabComponents, props.defaultTabComponent]);
    react_1.default.useEffect(function () {
        if (!dockviewRef.current) {
            return;
        }
        dockviewRef.current.updateOptions({
            createWatermarkComponent: props.watermarkComponent
                ? function () {
                    return new reactWatermarkPart_1.ReactWatermarkPart('watermark', props.watermarkComponent, {
                        addPortal: addPortal,
                    });
                }
                : undefined,
        });
    }, [props.watermarkComponent]);
    react_1.default.useEffect(function () {
        if (!dockviewRef.current) {
            return;
        }
        dockviewRef.current.updateOptions({
            createRightHeaderActionComponent: createGroupControlElement(props.rightHeaderActionsComponent, { addPortal: addPortal }),
        });
    }, [props.rightHeaderActionsComponent]);
    react_1.default.useEffect(function () {
        if (!dockviewRef.current) {
            return;
        }
        dockviewRef.current.updateOptions({
            createLeftHeaderActionComponent: createGroupControlElement(props.leftHeaderActionsComponent, { addPortal: addPortal }),
        });
    }, [props.leftHeaderActionsComponent]);
    react_1.default.useEffect(function () {
        if (!dockviewRef.current) {
            return;
        }
        dockviewRef.current.updateOptions({
            createPrefixHeaderActionComponent: createGroupControlElement(props.prefixHeaderActionsComponent, { addPortal: addPortal }),
        });
    }, [props.prefixHeaderActionsComponent]);
    return (react_1.default.createElement("div", { style: { height: '100%', width: '100%' }, ref: domRef }, portals));
});
exports.DockviewReact.displayName = 'DockviewComponent';
